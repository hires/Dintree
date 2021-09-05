/*
 * D101 Dual Envelope
 *
 * Copyright 2011: Dintree
 * Written by: Dintree
 *
 * Hardware I/O:
 *
 * 	RA0/AN0/PGD	- mode 2 switch - ADSR				- input - active low
 *  RA1/AN1/PGC	- mode 2 switch - LFO				- input - active low
 *	RA2/AN2		- sustain 1 pot						- analog
 *  RA3/MCLR	- gate 1 in							- input - active low
 *	RA4/AN3		- release 1 pot						- analog
 *	RA5			- gate 2 in							- input - active low
 *
 *	RB4/AN10	- attack 1 pot						- analog
 *	RB5/AN11	- decay 1 pot						- analog
 *	RB6			- DAC CLK							- SPI clock output
 *	RB7			- DAC !CS							- output - active low
 *
 *  RC0/AN4		- attack 2 pot						- analog
 *  RC1/AN5		- decay 2 pot						- analog
 *  RC2/AN6		- sustain 2 pot						- analog
 *  RC3/AN7		- release 2 pot						- analog
 *  RC4			- mode 1 switch - ADSR				- input - active low
 *  RC5			- mode 2 switch - LFO				- input - active low
 *  RC6/AN8		- test pin
 *  RC7/AN9		- DAC MOSI							- SPI data output
 *
 */
#include <system.h>
#include <flash.h>
#include "time_map.h"

// clock setting for delays
#pragma CLOCK_FREQ 8000000

// fuses
#pragma	DATA	_CONFIG,	_FCMEN_OFF & _IESO_OFF & _BOR_OFF & _CPD_OFF & _CP_OFF \
	& _MCLRE_OFF & _PWRTE_ON & _WDT_ON & _INTOSCIO

// hardware defines
#define MODE2_SW_ADSR porta.0
#define MODE2_SW_LFO porta.1
#define GATE1_IN porta.3
#define GATE2_IN porta.5
#define DAC_CS portb.7
#define MODE1_SW_ADSR portc.4
#define MODE1_SW_LFO portc.5
#define TEST_OUT portc.6

// maximum level for ramping up
#define MAX_LEVEL 65535
// envelope modes
#define MODE_ADSR 0
#define MODE_AR 1
#define MODE_LFO 2
unsigned char env_mode[2];  // the ADSR, AR, LFO mode
// envelope states
#define ENV_IDLE 0
#define ENV_ATTACK 1
#define ENV_DECAY 2
#define ENV_SUSTAIN 3
#define ENV_RELEASE 4
unsigned char env_state[2];  // the current ADSR state
// other stuff
unsigned char gate_state[2];  // the state of the gate
long env_level[2];  // the current DAC level for the output
unsigned int attack[2];  // step size for attack phase
unsigned int decay[2];  // step size for decay phase
unsigned int sustain[2];  // sustain level
unsigned int release[2];  // step size for release phase

// ADC stuff
unsigned char adc_chans[] = {10, 11, 2, 3, 4, 5, 6, 7};
unsigned char adc_vals[8];

// function prototypes
void load_dac(unsigned char chan, unsigned int val);
void reset_envelope(unsigned char chan);
void envelope_control(unsigned char chan, unsigned char gate);
unsigned int time_lookup(unsigned char val);

/*
 * Main.
 */
void main(void) {
	unsigned char chan_sel = 0;

	// initialize I/O
	osccon = 0x71;
	option_reg = 0x0f;  // weak pull-ups on, WDT 1:128
	wpua = 0x03;
	ansel = 0xfc;
	anselh = 0x0c;
	trisa = 0xff;
	porta = 0x00;
	trisb = 0x30;
	portb = 0x00;
	trisc = 0x3f;
	portc = 0x00;
	adcon0 = 0x01;
	adcon1 = 0x50;

	// SPI
	sspstat = 0x80;
	sspcon = 0x30;
	DAC_CS = 1;
	trisb.4 = 1;

	// timer 2 - 1024ms interval
	t2con = 0x3c;

	// clear outputs
	load_dac(0, 0);
	load_dac(1, 0);
	
	// kick off the ADC
	adcon0.GO = 1;

	// reset stuff
	env_mode[0] = MODE_ADSR;
	env_mode[1] = MODE_ADSR;
	env_state[0] = ENV_IDLE;
	env_state[1] = ENV_IDLE;
	gate_state[0] = 0;
	gate_state[1] = 0;
	env_level[0] = 0;
	env_level[1] = 0;
	attack[0] = 100;
	attack[1] = 100;
	decay[0] = 100;
	decay[1] = 100;
	sustain[0] = 4000;
	sustain[1] = 4000;
	release[0] = 100;
	release[1] = 100;

	// timer 2 int
	intcon = 0x00;
	intcon.PEIE = 1;
	pie1.TMR2IE = 1;
	intcon.GIE = 1;	

	// main loop!
	while(1) {
		clear_wdt();

		// read pot inputs
		if(!adcon0.GO) {
			intcon.GIE = 0;
			adc_vals[chan_sel] = adresh;
			intcon.GIE = 1;
			chan_sel = (chan_sel + 1) & 0x07;
			adcon0 &= 0xc3;
			adcon0 |= (adc_chans[chan_sel] & 0x0f) << 2;
			delay_us(50);  // acq time
			adcon0.GO = 1;
		}

		// process pot input to control variables
		intcon.GIE = 0;
		switch(chan_sel) {
			case 0:
				attack[0] = time_lookup(adc_vals[0] >> 1);
				break;
			case 1:
				decay[0] = time_lookup(adc_vals[1] >> 1);
				break;
			case 2:
				sustain[0] = adc_vals[2] << 8;
				break;
			case 3:
				release[0] = time_lookup(adc_vals[3] >> 1);
				break;
			case 4:
				attack[1] = time_lookup(adc_vals[4] >> 1);
				break;
			case 5:
				decay[1] = time_lookup(adc_vals[5] >> 1);
				break;
			case 6:
				sustain[1] = adc_vals[6] << 8;
				break;
			case 7:
				release[1] = time_lookup(adc_vals[7] >> 1);
				break;
		}
		intcon.GIE = 1;

		// handle mode switches
		if(!MODE1_SW_ADSR) {
			if(env_mode[0] != MODE_ADSR) {
				env_mode[0] = MODE_ADSR;
				reset_envelope(0);
			}
		}
		else if(!MODE1_SW_LFO) {
			if(env_mode[0] != MODE_LFO) {
				env_mode[0] = MODE_LFO;
				reset_envelope(0);
			}
		}
		else {
			if(env_mode[0] != MODE_AR) {
				env_mode[0] = MODE_AR;
				reset_envelope(0);
			}
		}

		if(!MODE2_SW_ADSR) {
			if(env_mode[1] != MODE_ADSR) {
				env_mode[1] = MODE_ADSR;
				reset_envelope(1);
			}
		}
		else if(!MODE2_SW_LFO) {
			if(env_mode[1] != MODE_LFO) {
				env_mode[1] = MODE_LFO;
				reset_envelope(1);
			}
		}
		else {
			if(env_mode[1] != MODE_AR) {
				env_mode[1] = MODE_AR;
				reset_envelope(1);
			}
		}
		delay_ms(10);
	}
}

// interrupt handler
void interrupt(void) {
	// timer 2 - 1ms interval
	if(pir1.TMR2IF) {
		pir1.TMR2IF = 0;
		TEST_OUT = 1;

		envelope_control(0, GATE1_IN);
		envelope_control(1, GATE2_IN);

		load_dac(0, (env_level[0] >> 4));
		load_dac(1, (env_level[1] >> 4));
		TEST_OUT = 0;
	}
}

// send output to DAC
void load_dac(unsigned char chan, unsigned int val) {
	DAC_CS = 0;
	sspstat.BF = 0;
	// send byte 0 - 0x30 (channel 0), 0x31 (channel 1)
	if(chan) sspbuf = 0x31;
	else sspbuf = 0x30;
	while(!sspstat.BF) clear_wdt();

	// send byte 1 - highest 8 bits
	sspbuf = (val >> 4);
	while(!sspstat.BF) clear_wdt();

	// send byte 2 - lowest 4 bits (left justified)
	sspbuf = (val << 4) & 0xf0;
	while(!sspstat.BF) clear_wdt();
	DAC_CS = 1;
}

// reset an envelope
void reset_envelope(unsigned char chan) {
	if(chan > 1) return;
	env_state[chan] = ENV_IDLE;
	env_level[chan] = 0;
}

// control an envelope
// gate signal is inverted - 0 = on, 1 = off
void envelope_control(unsigned char chan, unsigned char gate) {
	if(chan > 1) return;

	// 
	// GATE CONTROL
	//
	if(env_mode[chan] == MODE_LFO) {
		// restart the envelope if it is stopped
		if(env_state[chan] == ENV_IDLE) {
			env_state[chan] = ENV_ATTACK;
		}
	}
	else {
		// gate came on
		if(gate == 0 && gate_state[chan] == 0) {
			gate_state[chan] = 1;
			env_state[chan] = ENV_ATTACK;
		}
		// gate went off
		if(gate == 1 && gate_state[chan] == 1) {
			gate_state[chan] = 0;
			// only change to release phase for ADSR
			if(env_mode[chan] == MODE_ADSR) env_state[chan] = ENV_RELEASE;
		}
	}

	//
	// ENV STATE CONTROL
	//
	// attack
	if(env_state[chan] == ENV_ATTACK) {
		env_level[chan] += attack[chan];
		// is it time to move to the decay or release phase?
		if(env_level[chan] > MAX_LEVEL) {
			env_level[chan] = MAX_LEVEL;
			// ADSR mode
			if(env_mode[chan] == MODE_ADSR) {
				env_state[chan] = ENV_DECAY;
			}
			// AR or LFO mode
			else {
				env_state[chan] = ENV_RELEASE;
			}
		}
		return;
	}
	// decay
	if(env_state[chan] == ENV_DECAY) {
		env_level[chan] -= decay[chan];
		// is it time to move to the systain phase?
		if(env_level[chan] < sustain[chan]) {
			env_level[chan] = sustain[chan];
			env_state[chan] = ENV_SUSTAIN;
		}
		return;
	}
	// sustain
	if(env_state[chan] == ENV_SUSTAIN) {
		// make the sustain control interactive in realtime
		env_level[chan] = sustain[chan];
		return;
	}
	// release
	if(env_state[chan] == ENV_RELEASE) {
		env_level[chan] -= release[chan];
		// is it time to end the release phase?
		if(env_level[chan] < 0) {
			env_level[chan] = 0;
			env_state[chan] = ENV_IDLE;
		}
		return;
	}
}

// look up a time val and return the step size
unsigned int time_lookup(unsigned char val) {
	unsigned int temp;
	temp = flash_read(0x0800 + (val << 1));
	temp |= flash_read(0x0801 + (val << 1)) << 8;
	return temp;
}



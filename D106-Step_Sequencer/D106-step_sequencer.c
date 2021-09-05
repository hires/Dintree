/*
 * D106 Step Sequencer
 *
 * Copyright 2011: Dintree
 * Written by: Dintree
 *
 * Hardware I/O:
 *
 * 	RA0/AN0/PGD	- 
 *  RA1/AN1/PGC	- 
 *	RA2/AN2/INT	- clock in				- input - active low (interrupt)
 *  RA3/MCLR	- reset in				- input - active low
 *	RA4/AN3		- scrub in				- analog
 *	RA5			- dir in				- input - active low
 *
 *	RB4/AN10	- 
 *	RB5/AN11	- 
 *	RB6			- 
 *	RB7			- 
 *
 *  RC0/AN4		- step addr A			- output
 *  RC1/AN5		- step addr B			- output
 *  RC2/AN6		- step 1 LED			- output - active high
 *  RC3/AN7		- step 2 LED			- output - active high
 *  RC4			- step 3 LED			- output - active high
 *  RC5			- step 4 LED			- output - active high
 *  RC6/AN8		- 
 *  RC7/AN9		- 
 *
 */
#include <system.h>

// clock setting for delays
#pragma CLOCK_FREQ 8000000

// fuses
#pragma	DATA	_CONFIG,	_FCMEN_OFF & _IESO_OFF & _BOR_OFF & _CPD_OFF & _CP_OFF \
	& _MCLRE_OFF & _PWRTE_ON & _WDT_ON & _INTOSCIO

// hardware defines
#define RESET_IN porta.3
#define DIR_IN porta.5

#define CLOCK_TIMEOUT_TIME 2

unsigned char scrub_level;
unsigned char scrub_step;
unsigned char clock_timeout;
unsigned char seq_step;

// function prototypes
void set_step(unsigned char step);

/*
 * Main.
 */
void main(void) {
	// initialize I/O
	osccon = 0x71;
	option_reg = 0x0f;  // weak pull-ups on, WDT 1:128
	ansel = 0x08;
	anselh = 0x00;
	trisa = 0xff;
	porta = 0x00;
	trisb = 0x00;
	portb = 0x00;
	trisc = 0x00;
	portc = 0x00;
	adcon0 = 0x0d;
	adcon1 = 0x50;

	// timer 2 - 1024ms interval
	t2con = 0x3c;

	// reset stuff
	set_step(0);
	clock_timeout = 0;
	seq_step = 0;
	scrub_step = 0;

	// startup delay
	delay_ms(100);
	clear_wdt();
	delay_ms(100);
	clear_wdt();

	// timer 2 int
	intcon = 0x00;
	intcon.INTE = 1;
	intcon.PEIE = 1;
	pie1.TMR2IE = 1;
	intcon.GIE = 1;	

	// main loop!
	while(1) {
		clear_wdt();

		// read scrub input
		if(!adcon0.GO) {
			intcon.GIE = 0;
			scrub_level = adresh;
			intcon.GIE = 1;
			delay_us(50);  // acq time
			adcon0.GO = 1;
		}
	}
}

// interrupt handler
void interrupt(void) {
	unsigned char temp;

	// interrupt - clock in
	if(intcon.INTF) {
		intcon.INTF = 0;
		// step
		if(!clock_timeout) {
			clock_timeout = CLOCK_TIMEOUT_TIME;
			// only move if we're not resetting
			if(RESET_IN) {
				if(!DIR_IN) seq_step = (seq_step - 1) & 0x03;
				else seq_step = (seq_step + 1) & 0x03;
				set_step(seq_step);
			}
		}
	}

	// timer 2 - 1ms interval
	if(pir1.TMR2IF) {
		pir1.TMR2IF = 0;
		// handle clock timeout
		if(clock_timeout) {
			clock_timeout --;
		}
		// reset the sequencer
		if(!RESET_IN) {
			seq_step = 0;
			set_step(seq_step);
		}
		else {
			temp = scrub_level >> 6;
			if(scrub_step != temp) {
				scrub_step = temp;
				seq_step = scrub_step;
				set_step(seq_step);
			}
		}
	}
}

// set a step output
void set_step(unsigned char step) {
	if(step > 3) return;
	switch(step) {
		case 0:
			portc &= 0xc0;
			portc |= 0x04;
			break;
		case 1:
			portc &= 0xc0;
			portc |= 0x09;
			break;
		case 2:
			portc &= 0xc0;
			portc |= 0x12;
			break;
		case 3:
			portc &= 0xc0;
			portc |= 0x23;
			break;
	}
}
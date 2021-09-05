# D100 Quad VCA

** Quad Voltage Controlled Amplifier - 12HP Eurorack format **

![D100 Quad VCA](D100-front-600.jpg)

The D100 is a small quad VCA based around the SSM2164 VCA chip from Analog Devices. It's now obsolete but there are second-source versions and surplus stock available of this wonderful IC. The CV inputs operate from 0-5V to go from off to fully on. Both CV A and B inputs are mixed together. Each LEVEL pot is also mixed with the CV signal. This pot can adjust the range of the CV inputs by applying an offset either in the positive or negative direction. This is very useful for achieving the desired respose from envelope generators or other modulation sources. The pot can also simply be used to open and close the VCA manually which does not require a control signal to get some sound playing. The signal path is also completely DC coupled so it can be used for audio or control voltages, although the input control is logarithmic so it is less ideal for scaling control voltages.

## Circuit Design Video
[![IMAGE ALT TEXT](http://img.youtube.com/vi/dqyWzp-3qls/0.jpg)](http://www.youtube.com/watch?v=dqyWzp-3qls "Synth Tech - Quad VCA Design")
** Click to play **

## Technical Notes

The D100 Quad VCA is a simple yet high-quality VCA with a logarithmic CV response. The entire module uses just one SSM2164 VCA chip. You can't get these very easily anymore as of 2011, but the V2164 is a perfect clone made by CoolAudio. You should be able to use it in place of the SSM2164 with no problems.

Two CV inputs are used to drive each VCA allowing for things like tremolo by acting as a free CV mixer. These CVs are also mixed with a pot which allows manual offset control of the VCA, as well as tuning the VCA response by shifting the range of external CV control up and down.

The CV inputs are buffered and inverted. Because the SSM2164 operates with 0V meaning unity gain, and more attenuation with rising input voltage on the CV pin, inverting the CVs makes it work correctly for synthesizer use. Note that the VCAs will produce gain with voltages less than 0V, but we use a schottky diode to limit how negative the input to the CV pin can go. Only a small amount of gain is possible which prevents the output from clipping with normal input signals.

The final set of op amps convert the current output from the VCAs back into voltages for the outputs. Make sure not to omit the resistor and capacitors at the audio input to each VCA channel. They are required for stability as recommended by the manufacturer. Not shown is an important addition, which is a schottky diode connected reverse biased between ground and the negative power supply. This helps prevent damage to the SSM2164 if the module is ever powered up with a missing negative supply. In this case if the V- of the SSM2164 is left floating the chip can be destroyed by excessive current flow through the V+ pin.

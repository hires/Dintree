# D41 Violin Pre

## Description

The D41 Violin Pre is a simple project I designed and built in a few hours to solve a problem interfacing my NS Design NXT electric violin with other gear. It needs a high impedance input, but generates a lot of voltage which overloads most guitar pedals.

The D41 acts as a high impedance buffer, followed by a passive attenuator to make a strong line-level signal suitable for use with pedals or directly plugged into a mixer or other gear without a DI. The power consumption is very low. It runs on two 9V batteries which makes it a true split-rail design with more headroom than a standard single-supply 9V pedal. Putting the attenuator after the amplifier makes the noise level lower and lets you have more control over the tone by changing input part values.

Some changes you could try would be adjusting the value of C1 and R2,3. This will change the load on the pickup which will have an effect on the tone. Raising the value of C1 will block more bass. Lowering the value of R2,3 will adjust the overall loading. I started with 3M but found it sounded too bassy. Using 1.5M sounded right. Once you have a fairly flat signal you can easily add EQ after and there should be lots of signal to work with.

## Circuit Design Video
[![IMAGE ALT TEXT](http://img.youtube.com/vi/g0rNiV_gOls/0.jpg)](http://www.youtube.com/watch?v=g0rNiV_gOls "Simple Violin Preamp")
**Click to play**

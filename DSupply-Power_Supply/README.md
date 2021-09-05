# DSupply Power Supply

**Mini Low-cost Eurorack Power Supply**

## Circuit Design Video
[![IMAGE ALT TEXT](http://img.youtube.com/vi/B3IJ21pHkuw/0.jpg)](http://www.youtube.com/watch?v=B3IJ21pHkuw "Synth Tech - Eurorack Power Supply Design")

**Click thumbnail to play**

The DSupply is my third attempt at a Eurorack power supply and was designed to be small and inexpensive. It uses all off-the-shelf parts and can be mounted in a case or in a module faceplace. The input power jack, switch and LEDs can be mounted on either side of the PCB.

There is no +5V supply since this is not used by most modern modules and it's easy to adapt if needed with a plug-in unit. The input to the board is +15V and reasonable efficiency is obtained on the +12 and -12V outputs. This is suitable for powering 1-2 rows of average-sized modules. A separate power bus board or flying bus cable is used. These can be easily built from parts that you can get from a supplier like Digikey or Mouser.

The video above should explain the rest. For reasons I don't provide the PCB layout, since it's a project I might still build and sell later. But the schematic is free to use for non-commercial designs or for learning. This circuit will likely not work on a prototyping board, so lay out a 2-layer PCB with lots of ground plane and a similar tight arrangement of parts as you see in the video. I was able to use mostly through-hole parts except for the MOSFETs and actual DC-DC controller ICs. These are easy to hand-solder so don't be afraid!

Note: We do not have any PCBs available and I don't expect to be making any. If you want to build one there are lots of low-cost places to get your own PCBs made.

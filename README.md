# Dintree

**Synthesizer Modules and Audio Circuits**

**2020-07-14: You can now try Dintree modules within [VCV Rack](https://library.vcvrack.com/?query=dintree&brand=&tag=&license=)! I have created virtual versions of most modules.**

Here are some DIY synth projects, complete with schematics and code where applicable. All the projects here are for learning and hobby use and are strictly for education and **NON-COMMERCIAL USE ONLY**.

![Dintree](dintree-logo.png)

## About Me

My name is Andrew Kilpatrick and I'm more well-known for my full-scale synthesizers and sequencers sold under the [Kilpatrick Audio](https://www.kilpatrickaudio.com) brand. The ideas presented here are intended as a free resource to help those starting out. I definitely benefitted from a lot of resources while learning and feel now like it's my turn to pass it on my knowledge.

This site is my own responsitory of free info and this a completely sole venture that doesn't have anything to do with my commercial brand or products. Sadly there seem to be unscrupulous people out there who claim they have some affiliation with me and are selling direct copies of these projects under their own names. Please do not buy these products.

There are many hard-working synth designers out there like myself who put literally all our time and effort into making our ideas available for musicians around the world. If you want something pre-made, please buy one of our products! That's how we pay our bills and stay enthused about making cool stuff! If you want to do some of the work and save a few bucks, buy a kit. I don't offer kits but there are many very good kit makers out there... please support them. If you're more of a DIY person that wants to really get their hands dirty and learn the entire process, then that's what this site is for! Please go and hunt down the parts, learn to make boards, gather the parts and make something awesome. It will be one of the most rewarding experiences you have to make music with gear that you made yourself from scratch.
Need help?

I'm pretty busy but you can contact me at: dintree-mail@andrewkilpatrick.org. Keep in mind that this info is free and although I love to help teach people about electronics, I don't have time for every kind of request, including:

- **I can't design stuff for you** - My schedule is full with other work so if you want to make something, just learn what you need and make it
- **I don't offer kits** - This is tedious and time consuming and I make my living by the generous support of my customers who buy my finished products which are designed and built to a very high standard.
- **I generally don't offer PCBs for anything** - The idea of this site is to give you a head start. There are plenty of DIY synth kits available online. The goal here is to hopefully explain some of the more technical details so you can learn to design your own stuff.

## General Technical Tips

A few notes that apply to most modular synth circuits, which you will see throughout the projects below. These are good design practice for most circuits used in synths and other audio equipment:

- Always use reverse protection diodes in the power supply. If something gets hooked up backwards, it's nice to know that you won't fry all your chips.
- Input termination / impedances must be within the range of 20K to 100K. This is important for modular synths since it means that multiple inputs can be driven from a single output, and they won't load down the output too much. I tend to choose something between 20K and 50K for audio and non-critical control inputs, because this range is fairly immune to noise pickup even from unshielded cables. For pitch inputs on VCOs and the like, make sure to use 100K input impedances to avoid introducing tuning errors from excessive input loading.
- Output impedances should be around 1K by placing a series resistor in the output. This means that even if you short an output to ground or even one of the power supplies, the current through the output opamp is limited. Most amps have limited output current, but they can still be destroyed by overheating themselves. For most audio and CV outputs 1K is a good value that can still drive many inputs. For precision CV outputs used for pitch like from a keyboard or MIDI interface, use 100 ohms to minimize pitch errors.
- Outputs should always be terminated when the module is unpowered. Semiconductors are usually floating when off, so if you unpower your circuit while it is driving an external device like a mixer, you will now have an unterminated cable picking up all kinds of noise. I use 100K resistors to ground on outputs. Put them before the output series resistor to avoid unnecessary voltage drops. Trick: If you follow the output back around the output opamp and find that there is a complete resistive path to ground somewhere you can avoid an extra resistor! This happens in non-inverting configurations quite a lot, so check it before adding more parts.
- In amplifiers with high gain settings, or amplifying signals from the outside world you should always put a small capacitor across the feedback path. For feedback resistors of less than 50K or so, I usually choose 100pF. For 100K or so, use 22pF. These values will reduce the gain at very high frequencies thus reducing the chance of oscillation, or amplifying RF noise, while also not overly attenuating actually high frequency audio signals. Check it on your scope to be sure. It's good practice to do on all amplifiers, but sometimes it's not really necessary.
- Always place 0.1uF ceramic capacitors directly from each power supply pin to ground on every opamp and other IC. These improve stability and bypasses high frequency noise to ground. Don't just bridge both supply pins with a single cap... it doesn't work and can actually cause more problems. Unless your power and ground wiring is neat and there are 0.1uF capacitors all over your circuit at every IC, don't even think about trying to test your circuit.
- Always put some bulk filtering on every module. For small analog circuits 10uF on each supply is okay after the input diode. For higher power stuff 100uF or more might be required. But don't go crazy because large capacitors can store quite a lot of energy and become a liability, drawing excessive current at power on, and sometimes causing certain types of switching power supplies to freak out.

## Important Notes about Commercial Use / License
If you want to make your own project, that's cool. Even if you want to sell it. But please keep the following in mind:

- These schematics are for **NON-COMMERCIAL USE ONLY!** - These are mostly little weekend projects I make to solve problems in my own setup. They are simple but functional, yet are not designed or intended for use directly as commercial products.
- Make something derivative - Make your own design based on what you learn here, or adapt my circuit for your own use. This is cool. Send me a link to a picture or video when you're done!
- Change the name - Call it something that you come up with. I found someone selling one of these designs with the exact same model number and name. I felt kind of upset about it. So just make your own product name, logo, style.
- Share what you make - Make a video or website showing what you made and maybe even offer your designs as inspiration for others! Have a hackathon or make a kit for sale to others starting out! I wish I had time to do some of these things.
- Review the design - Circuits built for your own use are often much simpler than what is necessary for a professional / commercial design. This includes topics such as ESD protection, EMC, design for manufacture (DFM) and so on. It's usually quite a lot of effort to go from a weekend project to a manufacturable product that will survive the real world. Do your own research and consult with industry professionals to learn what are acceptable standards of design and testing for the type of product you are making.

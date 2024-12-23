# Locomotive Decoder

This page describes how to build RCC Locomotive Decoder and how to use one. 

RCC supports several types of decoders with different pros and cons. All of them are built out of just two components - the motherboard and the CPU. Yep, your decoder will have a motherboard, like a grown up computer. The good news about RCC design - all decoders are using the same motherboard. Once you decide to try RCC - go ahead and fabricate a couple boards. How cool is that! You’d build your own boards! Detailed instruction is in [fabrication](fabrication).

Now you need to select a CPU. This is mostly driven by type of a Throttle you want and desired complexity of your setup. Following sections show the available options. Once you have selected your CPU, buy one and solder it to the motherboard. Here is that scary word - solder. But fear not, this is fairly simple process anyone can master following this instructions - [how to solder your CPU](soldering).

## Specs
This is how the assembled RCC Decoder looks like:
![Rcc Decoder look](image/look.jpg)
The decoder can drive a motor for up to 3Amp. It has 6 outputs that can drive lights, smoke unit and similar, 0.5Amp each and it has sound. Everything you can possible want out of a decoder. If this is not enough, here is the punch line. Five fully assembled boards would cost you $60 (plus shipping, say $20). So, it is about $16 per board. The CPU would be $10 per unit. So, you can get a fully functional decoder with sound and eveything for under $30, shipping and taxes included! Nothing this powerfull is remotly this affordable.

## Throttle connection

### Simple WiThrottle

This is the best choise if you want to run just couple locomotives, usually one at a time, and you want to keep everything really simple. You may heard about JMRI or DCC-EX but you don't know what it is and you don't want to know. You just want to have fun. What you would need is the RCC Decoder and a phone or a tablet as a Throttle. Recomennded CPU for this setup is Xiao ESP32C3 (you can get one from [Seed Studio](tbd) or [Amazon](tbd)). Actually any of Xiao ESP32 boards will do (C3, C6, S3), but C3 is the cheapest and it works just fine.






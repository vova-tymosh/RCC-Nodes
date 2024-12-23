# Locomotive Decoder

This page describes how to build RCC Locomotive Decoder and how to use one. 

RCC supports several types of decoders with different pros and cons. All of them are built out of just two components - the motherboard and the CPU. Yep, your decoder will have a motherboard, like a grown up computer. The good news about RCC design - all decoders are using the same motherboard. Once you decide to try RCC - go ahead and fabricate couple boards. How cool is that! You’d build your own boards! Detailed instruction is in [fabrication](fabrication).

Now you need to select a CPU. This is mostly driven by type of a Throttle you want and desired complexity of your setup. Scroll down to **CPU Selection** to learn more. Once you have selected your CPU, buy one and solder it to the motherboard. Here is that scary word - solder. But fear not, this is fairly simple process anyone can master following this instructions - [how to solder your CPU](soldering).

## Specs
 - Power Supply: battery of 7v to 24v
 - Motor Power: RCC decoder can drive motor up to 3Amp
 - Functions: RCC decoder can control up to 6 external functions like lights or smoke unit
 - Sound: _coming soon_ motherboard is capable of playing sounds, software support will be available soon
 - Price: this is one of the best parts. Five fully assembled boards would cost you $60 (plus shipping, say $20). So, it is about $16 per board. The CPU is $10 per unit. So, you can get a fully functional decoder with sound and eveything for under $30, shipping and taxes included! Nothing this powerfull is remotly this affordable.


## CPU Selection
RCC Decoder is based on a [Xiao from Seeed Studio](https://www.seeedstudio.com/xiao-series-page). These are the tiny modules and RCC supports veriaty of them with WiFi or BLE connectivity.
![Xiao](image/XIAO.jpg) 

### Simple WiThrottle

This is the best choise if you want to run just couple locomotives, usually one at a time, and you want to keep everything really simple. You may heard about JMRI or DCC-EX but you don't know what it is and you don't want to know. You just want to have fun. What you would need is the RCC Decoder and a phone or a tablet as a Throttle. Recomennded CPU for this setup is Xiao ESP32C3 (you can get one from [Seed Studio](tbd) or [Amazon](tbd)). Actually any of Xiao ESP32 boards will do (C3, C6, S3), but C3 is the cheapest and it works just fine.

1. Install Arduino IDE
2. Add esp32 support
3. Connect Antena
4. Download sw from [locoWT](tbd)
5. Start IDE, select port/board
6. Update wifi, upload

### WiFi + JMRI

This is sligtly more complicated setup. You may still use a phone or a tablet as a Throttle, and you'd be able to run many locomotives at the same time. The complexity comes from a Central Station. You would need to build one. More details on how to do so on [RCC Station](tbd) page. 
The same Xiao ESP32C3 (or C6 or S3) can be used for CPU. 

1. Install Arduino IDE
2. Add esp32 support
3. Connect Antena
4. Download sw from [locoJMRI](tbd)
5. Start IDE, select port/board
6. Update wifi, upload


### NRF

This is more complicated option, the main benefit is versitility. It may work with or without command station, it doesn't need WiFi. But it does require [RCC Throttle](https://github.com/vova-tymosh/RCC-Nodes/tree/main/Throttle).

2. Install Arduino IDE
3. Add nrf52 support
4. Download sw from [locoNRF](tbd)
5. Start IDE, select port/board
6. Upload








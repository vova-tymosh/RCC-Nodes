# Locomotive Decoder


This page describes how to build a RCC Locomotive Decoder and how to use one.


RCC decoder is built out of just two components - a motherboard and a CPU (yep, your decoder will have a motherboard and a CPU, like a grown up computer). You’d need to order your motherboard, solder a CPU to it and finally, connect a locomotive motor, speaker, lights and other things to the decoder and you are good to go.



## Step-by-step instructions


1. Download the latest [RCC release](https://github.com/vova-tymosh/RCC-Nodes/releases) - take the topmost RccDecoder_vxxx.zip file. Unzip it somewhere on your computer.
2. You need a CPU, one per decoder. The RCC is built around Seeed Studio Xiao module. You can get a Xiao ESP32C3 from [Seed Studio](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/) or [Amazon](https://www.amazon.com/Seeed-Studio-XIAO-ESP32C3-Microcontroller/dp/B0B94JZ2YF/ref=asc_df_B0B94JZ2YF?mcid=72b6a39f01243f90839a6151057a3d9f&hvocijid=9306890703710665590-B0B94JZ2YF-&hvexpln=73&tag=hyprod-20&linkCode=df0&hvadid=721245378154&hvpos=&hvnetw=g&hvrand=9306890703710665590&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1013950&hvtargid=pla-2281435178138&th=1). There are other options available, and you can read more on [CPU Selection](cpu_selection.md) page.
3. You need a motherboard for your decoder. Use the Gerber_RccDecoder.zip, BOM_RccDecoder.csv and CPL_RccDecoder.csv from the release you downloaded at step 1 and send them to jlpcb.com (or similar vendor). Step-by-step instructions are on [Fabrication](fabrication).
5. Upload software to the final decoder, the details about how to do that is on the [Upload](upload.md) page.
6. By default the decoder starts as a WiFi access point with the name “RCC_Loco” and the same password. You may keep it this way or you may connect your decoder to your home WiFi. More details setting the decoder up are on the [First Setup](first_setup.md) page.
4. Now it's time to solder the CPU to the motherboard. Here is that scary word - solder. But fear not, it is a fairly simple process and I’ll provide instructions and videos (I promise, I’ll do it soon).
7. Your decoder is ready to be placed in a locomotive. Go ahead and do that, more details about how to connect all the wires are on the [Install](install.md) page.
8. Get a Throttle app on your mobile device - "Engine Driver" for Android or "WiThrottle" for iPhone. Any other app that supports WiThrottle protocol will do too.
9. Start the app, connect to your locomotive decoder, enjoy!


## Decoder Specs  
* Power Supply: battery 7v to 24v.
* Power: RCC decoder can drive a motor up to 3 Amp.
* Functions: RCC decoder can control up to 6 external functions like lights, smoke units or similar. Each output/function can handle up to 0.5 Amp load.
* Sound: (_coming soon_) motherboard is capable of playing sounds, software support will be available soon.
* Price: this is one of the best parts. Five fully assembled boards would cost you $60 (plus shipping, say $20). So, it is about $16 per board. The CPU is $10 per unit. So, you can get a fully functional decoder with sound and everything for under $30, shipping and taxes included! Nothing this powerful is remotely this affordable.



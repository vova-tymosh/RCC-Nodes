# Locomotive Decoder


This page describes how to build a RCC Locomotive Decoder and how to use one.


RCC decoder is built out of just two components - a motherboard and a CPU (yep, your decoder will have a motherboard and a CPU, like a grown up computer). You’d need to order your motherboard, solder a CPU to it and finally, connect a locomotive motor, speaker, lights and other things to the decoder and you are good to go.



## Step-by-step instructions


1. As of now only one technology stack and a corresponding CPU is supported. It is enough to run a couple trains using your phone as a throttle. The CPU is the Seeed Studio Xiao module. Go ahead and buy Xiao ESP32C3 (you can get one from Seed Studio or Amazon). More options are coming, see [CPU Selection](cpu_selection.md) for more details. You’d need one CPU per decoder/loco.
2. Download the latest release of RCC from #here#. Unzip it somewhere on your computer.
3. Order motherboard for your decoder. Use the 3 files from the fabrication sub-folder of the release you just downloaded and send them to jlpcb.com (or similar vendor). Step-by-step instructions are on [fabrication](fabrication). You’d need one motherboard per decoder/loco.
4. Solder the CPU to the motherboard. Here is that scary word - solder. But fear not, it is a fairly simple process and I’ll provide instructions and videos (I promise, I’ll do it soon).
5. Upload software to the final decoder, the details about how to do that is on the [Upload](upload.md) page.
6. By default the decoder starts as a WiFi access point with the name “RCC_Loco” and the same password. You may keep it this way, especially if you plan to use an old phone or a tablet that is always connected to this WiFi and you don’t need internet access on this phone/tablet. Alternatively, you may connect your decoder to your home WiFi. In this case you may use your main phone, as it would be connected to the internet as well as to a decoder. More details are on the [First Setup](first_setup.md)
7. Your decoder is ready to be placed in a locomotive. Go ahead and do that, more details about how to connect all the wires are on the [Install](install.md) page.
8. Get a Throttle app on your mobile device - "Engine Driver" for Android or "WiThrottle" for iPhone. Any other app that supports WiThrottle protocol will do too.
9. Start the app, connect to your locomotive decoder, enjoy!


## Decoder Specs  
* Power Supply: battery 7v to 24v.
* Power: RCC decoder can drive a motor of up to 3 Amp.
* Functions: RCC decoder can control up to 6 external functions like lights, smoke units or similar. Each output/function can handle up to 0.5 Amp load.
* Sound: (_coming soon_) motherboard is capable of playing sounds, software support will be available soon.
* Price: this is one of the best parts. Five fully assembled boards would cost you $60 (plus shipping, say $20). So, it is about $16 per board. The CPU is $10 per unit. So, you can get a fully functional decoder with sound and everything for under $30, shipping and taxes included! Nothing this powerful is remotely this affordable.

# CPU Selection
RCC Decoder is based on the Seeed Studio Xiao modules, you can read more about them [here](https://www.seeedstudio.com/xiao-series-page). These are really tiny micro computers, like a thumb nail, compatible with Arduino. RCC supports several variants with WiFi, BLE/NRF connectivity. You'd likely need a WiFi solution, as it is more versatile and easier to build. Here is how this CPU, the Xiao module, look like:
![Xiao](image/XIAO.jpg)


## WiFi (recommended)
Everyone has WiFi connectvity at home. You can connect your Loco to that WiFi network or use your Loco as an access point (Loco would have its own WiFi network). You can use WiThrottle protocol running right on the decoder, in such case you'd need a phone/tablet as a Throttle/Controller and nothing else. Alternativly you can setup JRMI and control many locomotives and other railroad elements through JMRI.
Recommended CPU for this setup is Xiao ESP32C3, you can get one from [Seed Studio](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/) or [Amazon](https://www.amazon.com/Seeed-Studio-XIAO-ESP32C3-Microcontroller/dp/B0B94JZ2YF/ref=asc_df_B0B94JZ2YF?mcid=72b6a39f01243f90839a6151057a3d9f&hvocijid=9306890703710665590-B0B94JZ2YF-&hvexpln=73&tag=hyprod-20&linkCode=df0&hvadid=721245378154&hvpos=&hvnetw=g&hvrand=9306890703710665590&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1013950&hvtargid=pla-2281435178138&th=1). Actually any of Xiao ESP32 boards will do - C3, C6 or S3. But if you select other boards you'd need to recompile code in Arduino Studio yourself.


## NRF (Only if you know what you are doing)
This is the more complicated option and it offers the most versatility. It may work with or without a central station, it doesn't need WiFi. But it does require a hardware Throttle. How to build one is explained on the RCC Throttle page (_still under construction_). I’m using this setup to control my live steam locomotives. I run them at home and at various events. This option allows me to do both without any changes or complications.

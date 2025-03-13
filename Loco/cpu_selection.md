# CPU Selection
RCC Decoder is based on the Seeed Studio Xiao modules, you can read more about them [here](https://www.seeedstudio.com/xiao-series-page). These are really tiny micro computers, the size of your thumb nail. RCC supports several variants with WiFi or BLE connectivity. Which one you need depends on how complicated you want your setup to be. Following sections provide details for each, below are the pictures of the CPU (the Xiao module).
![Xiao](image/XIAO.jpg)


## Simple (WiThrottle)
If you are not sure which option to choose, choose this one. This is the best choice if you want to run just a couple locomotives, usually one at a time and you want to keep everything really simple. You may have heard about JMRI or DCC-EX but you don't know what it is and you don't want to know. You just want to have fun. What you would need is the RCC Decoder and a phone or a tablet as a Throttle. Recommended CPU for this setup is Xiao ESP32C3, you can get one from [Seed Studio](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/) or [Amazon](https://www.amazon.com/Seeed-Studio-XIAO-ESP32C3-Microcontroller/dp/B0B94JZ2YF/ref=asc_df_B0B94JZ2YF?mcid=72b6a39f01243f90839a6151057a3d9f&hvocijid=9306890703710665590-B0B94JZ2YF-&hvexpln=73&tag=hyprod-20&linkCode=df0&hvadid=721245378154&hvpos=&hvnetw=g&hvrand=9306890703710665590&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1013950&hvtargid=pla-2281435178138&th=1). Actually any of Xiao ESP32 boards will do - C3, C6, S3. The C3 is the cheapest and it works just fine. The good part about this choice is flexibility. If you would decide to go complicated, run many trains, control turnouts and such - you would only need to update the software, the hardware can still be the same hardware.




## Complex (MQTT)
This is a more complicated setup. You may still use a phone or a tablet as a Throttle, and you'd be able to run many locomotives at the same time. The complexity comes from a Central Station. You would need to build one and install JMRI. More details on how to do so is on the RCC Station page (_coming soon_). The CPU selection for this option is the same as above, Xiao ESP32C3.




## Flexible (NRF)
This is the most complicated option and it offers the most versatility. It may work with or without a command station, it doesn't need WiFi. But it does require a hardware Throttle. How to build one is explained on the RCC Throttle page (_still under construction_). I’m using this setup to control my live steam locomotives. I run them at home and I also meet with my fellow live steamers at various events. This option allows me to do both without any changes or complications.

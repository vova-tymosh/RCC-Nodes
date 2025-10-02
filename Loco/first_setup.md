# First Setup

The First Setup goes after you upload your software to the decoder. You can do this before you solder the CPU or after.

1. Power on your decoder (connect USB or if the CPU is coldered, connect the battery to the board).
2. Using your phone, tablet or computer connect to _RCC_Loco_ WiFi. Password is also _RCC_Loco_.
3. Open browser and go to http://192.168.4.1
4. Here is what you’d see:
![rcc web config](image/RCCWebConfig.png)

5. Chose if you want to run your decoder on its own WiFi, or to be connected to your home network:
   - If you want the decoder to have its own WiFi, to be an access point, change the "wifiap" line to "ON". You may change WiFi name (WiFiSSID) and password if you want to. If you do change it you’d need to reconnect your phone/tablet/computer to this WiFi after you are done. This mode is good if your layout is outside of your home WiFi. The only downside is your phone/tablet/computer will not have internet access while it is connected to this WiFi. Might be a good idea to use an old device just as a throttle (always connected to this WiFi only).
   - Alternatively, you may connect your decoder to your home network. For that type "OFF" in eth "wifiap" field. After that provide your home WiFi SSID and password. Just make sure you are connecting to the 2.4GHz WiFi. The 5GHz is not supported.
6. If you keep "mqtt" "OFF" the loco will work in WiThrottle mode. In this mode you connect your WiThrottle compatible app directly to the Loco. If you flip it "ON" you'd also need to privide "broker", the IP address of your MQTT broker.
7. The LocoName and LocoAddress are a DCC-like name and address to identify your loco in the app. Default name is RCC and address is 3. If you put an address greater than 127 it will show in your WiThrottle app as a Long address. Smaller numbers will be shown as Short.
8. Hit Submit and then power cycle the decoder.


If you selected "NO" for "mqtt" (step 6) your Loco decoder would work in WiThrottle mode. You'd need an app that speaks WiThrottle (like _Engine Driver_ for Android and _WiThrottle_ for iOS) as your Throttle/Controller. This is the simplest setup. You don't need anything else. The downside of this - you'd be able to control only one Loco at a time. It is possible to control multiple locomotives, but to switch between them you’d need to go all the way to the first screen of the app and select another WiThrottle server. Unfortunately, this limitation would not allow you to put several locomotives on the same app screen. To control your Loco make sure your phone/tablet is connected to the same WiFi you have set up on the step 5 above. You should see the same name you have used on step 7 above as an available WiThrottle server in the app. Click on it. Now you need to select your locomotive in the app, the same way you’d do that if you use DCC or JMRI or any other solution. To do so type the address (whatever was provided in the step 7 above) or just select the locomotive from the roster. Here you go - you are ready to control the loco/decoder.

If you want to quickly swich between locos you'd need to select "ON" for "mqtt" (step 6) and you'd need to install (JRMI)[https://www.jmri.org/]. I'll provide more details for the setup soon.



# First Setup

Here is how to configure your decoder.

1. Power on your decoder (connect USB or if the CPU is already soldered, connect the battery to the board).
2. Using your phone, tablet or computer connect to _RCC_Loco_ WiFi. Password is also _RCC_Loco_.
3. Open browser and go to http://192.168.4.1
4. You will see the list of configuration values. They are similar to DCC CV values.
5. Change the settings as per instructions below and click Submit.

## Loco identification

**locoaddr** - Loco address like in DCC, default is 3. If you put an address greater than 127 it will show in your WiThrottle app as a Long address. Smaller numbers will be shown as Short.

**loconame** - Loco name which would be shown on your Throttle/Controller. Default is RCC.


## Access Point vs Home Wifi

There are two modes of operation with regards to WiFi. A loco may work as its own access point or it can be connected to your home WiFi. The first case is better if your layot is outside of your home WiFi range. The second one is better if you have multiple locomotives and other rairoad elements that you want to control.

**wifiap** - Set this parameter to ON in case you want Loco to run as its own network. Set it OFF if you want to connect it to your home WiFi. Make sure you are connecting to the 2.4GHz WiFi, the 5GHz is not supported.

**wifissid** - This is the name of the WiFi network. In case **wifiap** is ON this is the name of your Loco network (default is _RCC_Loco_). If **wifiap** is OFF this would be the name of your home wifi.

**wifipwd** - This is the password of the WiFi network. In case **wifiap** is ON this is the password of your Loco network (default is _RCC_Loco_). If **wifiap** is OFF this would be the password of your home wifi.


## MQTT vs WiThrottle

There are two protocols RCC supports - WiThrottle and MQTT. WiThrottle is simpler solution, it doesn't need central station. You'd need the RCC decoder and an app that speaks WiThrottle. You may find the list of such apps on [JRMI WiThrottle](https://www.jmri.org/help/en/package/jmri/jmrit/withrottle/UserInterface.shtml) page. This protocol was designed for command stations like DCC-EX. In case of RCC it runs right on the decoder itself. This creates a limitation - you would be able to control only one locomotive at a time. It is possible to switch between locomotives, it's just less conviniet. To switch you'd have to reconnect to another WiThrottle (which runs on another locomotive).

If you want to control multiple locomotives and other railroad equipment you'd need JRMI. You may still use the same apps or any other apps that talks to JRMI. More details you may find on the [JRMI](https://www.jmri.org/) page). The protocol that RCC uses to comunicate with JRMI is MQTT. Learn more about this on [JRMI MQTT](https://www.jmri.org/help/en/html/hardware/mqtt/index.shtml) page.


**mqtt** - Set this to ON to use JRMI/MQTT or OFF to use WiThrottle.

**broker** - Used only if **mqtt** is ON, this is IP address of an MQTT broker.

**brokerport** - Used only if **mqtt** is ON, this is a port of the an MQTT broker, usually 1883.



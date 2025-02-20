# Fabrication

This page describes how to fabricate a motherboard for an RCC locomotive decoder. A motherboard is a board with some electronics on it. The more correct name for such things is Printed Circuit Board (PCB). I'm using JLPCB to do the manufacturing, you may try some other vendor and the steps would be very similar.


1. You would need 3 files from the release you should have downloaded already - Gerber_RccDecoder.zip, BOM_RccDecoder.csv and CPL_RccDecoder.csv.
2. Go to [jlcpcb.com](https://jlcpcb.com).
3. Click **Instant Quote** and then **Upload Gerber**.
5. Select the **Gerber_xx** file you got in the step 1.
6. Most of the settings below are good as they are. Only check the following:
    - **PCB Qty** - how many boards do you need, minimum is 5
    - **PCB Color** - choose the board color
    - **Mark on PCB** - select **Remove Mark**, you don't want extra marks from the manufacturer.
8. Go down to **PCB Assembly**, enable it. It may ask you about panelization, say **I don't need panelization**.
9. More settings will appear. Check these:
    - **PCBA Qty** - how many of the boards you want fully assembled, the minimum here is 2. This means they will build 2 fully assembled motherboards and the rest 3 wouldn't have electronics on them. That would be absolute minimum quantity.
    - **Tooling holes** - select **Added by Customer**.
    - **Board Cleaning** - this is optional, if you like your boards to looks nicer - select **Yes**.
10. Click **Next**. It will show you how the PCB would look like. CLick **Next** again.
11. Now it asks you for 2 more files - give them **BOM_xx** and **CPL_xx**.
12. You'll see the error message with the following text. This is normal, these are not real components but just pads. Say **Continue**.
```
Error
The below parts won't be assembled due to data missing.
X1,X2,X3,X4,X1,X5,X6,X7 designators don't exist in the BOM file.
```
13. Now you see the list of your components. If everything is still in stock all the rows should be checked in the last column.
    - If you, like me, don't want the screw terminal and prefer dupont connectors, unselect items J1 and J2.
14. Click **Next**, go to payment, select your shipping and boom! The boards will arrive at your door soon.

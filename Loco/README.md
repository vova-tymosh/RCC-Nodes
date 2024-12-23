# Locomotove controller

1. Download all 3 files from [fabrication](fabrication) folder above, 
2. Go to (jlcpcb.com)
3. Click **Instant Quote** and then **Uplolad Gerber**
5. Selecte the **Gerber_xx** file you donwloaded in the step 1
6. Most of the seting below are good as they are. Only check the following:
    - **PCB Qty** - how many boards do you need (minimum is 5)
    - **PCB Color** - choose the color
    - **Mark on PCB** - select **Remove Mark**, you don't want extra marks from the manufacturer
8. Go down to **PCB Assembly**, enable it. It may ask you about panelization, say **I don't need panelization**
9. More seting will appear. Same as the above most of them are ok, only check these:
    - **PCBA Qty** - if you only want to try on 1-2 locos go with minimum amount in step 6 (5 boards) and minimum amount here, which is 2. This means they will build 2 fully assembled mother boards for you and the rest 3 wouldn't have electornics on them. You can throw these 3 away.
    - **Tooling holes** - selecte **Added by Customer**
    - **Board Cleaning** - this is optional, if you like your boards to looks nicer - select **Yes**.
10. Click **Next**. It will show you how the PCB would look like. CLick **Next** again.
11. Now it asks you for 2 more files - select **BOM_xx** for the BOM one and **CPL_xx** for the CPL one.
12. You'll see the error message with the following text. This is normal, these are not real components but just pads. Say **Continue**.
    ```The below parts won't be assembled due to data missing.
X1,X2,X3,X4,X1,X5,X6,X7 designators don't exist in the BOM file.```
13. Now you see the list of your components. If everything is still in stock all the rows should be checked in the last column.
    - If you, like me, don't want this screw terminal and prefer dupont connectors, unselect items J1 and J2
14. CLick **Next**, go to payment, select your shipping and boom! The boards will arrive to your door soon.

   

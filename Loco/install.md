# Install

At this point you should have your decoder ready to be installed in the locomotive. You got your motherboards fabricated, CPU soldered and the software uploaded. This step is trivial - figure what wires on your locomotive are connected to the motor, smoke unit, lights and such and connect them to the decoder. If you connected some other decoders (DCC or radio) before - you know what to do.

Grab a screwdriver and just connect the right wires. Each motherboard terminal has a markings on the back:

![back](image/decoder_back.jpg)

Left side:
 * 1 to 6: functions to control lights, smoke units or similar things. Connect one wire to the function and another to the Vc connector below. If the load is polarity sensitive (like LED lights) connect positive terminal to the Vc and negative to the numbered output.
   * Function 5 is a high load one, it can do 1Amp, other functions are 0.5Amp.
   * Function 6 has a pull up resistor (I'll provide details and reasons at some point later).
 * S+ and S-: for the speaker "+" and "-" terminals

Right side:
 * Power (+ and -): connect battery "+" and "-" terminals here. Battery voltage can be anything between 7V and 24V. BE CAREFUL. This is the only thing that can fry your decoder if you do it wrong.
 * Speed Sensor: Ground, +5V and Signal to connect optical sensor to measure locomotive speed. This is optional, more details will come later.
 * Motor: M+ and M- terminals for the locomotive motor. It is not really critical which one is "+" and which is "-". Nothing will burn if you connect them wrong, just the locomotive will go reverse when the Throttle says forward. If this happens - swap the "+" and "-" connections.
 



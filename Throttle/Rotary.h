#pragma once
#include <RotaryEncoder.h>


void rotaryUpdate();

class Rotary
{
    int pinA, pinB;
    int lastPos;
public:
    RotaryEncoder encoder;

    Rotary(int pinA, int pinB) : pinA(pinA), pinB(pinB), encoder(pinA, pinB, RotaryEncoder::LatchMode::TWO03) {}

    void begin()
    {
        attachInterrupt(digitalPinToInterrupt(pinA), rotaryUpdate, CHANGE);
        attachInterrupt(digitalPinToInterrupt(pinB), rotaryUpdate, CHANGE);
    }

    void loop()
    {
        encoder.tick();
    }

    int read()
    {
        lastPos = encoder.getPosition();
        return lastPos;
    }

    int readAccelerated()
    {
        int newPos = encoder.getPosition();
        if (abs(newPos - lastPos) > 3) {
            int deltaTicks = 10 * (newPos - lastPos);
            newPos = newPos + deltaTicks;
            encoder.setPosition(newPos);
        }
        lastPos = newPos;
        return newPos;
    }
};
extern Rotary rotary;

void rotaryUpdate()
{
    rotary.encoder.tick();
}

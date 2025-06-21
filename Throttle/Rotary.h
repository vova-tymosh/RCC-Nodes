#pragma once
#include <RotaryEncoder.h>


void rotaryUpdate();

class Rotary
{
    int pinA, pinB;
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
        return encoder.getPosition();
    }
};
extern Rotary rotary;

void rotaryUpdate()
{
    rotary.encoder.tick();
}

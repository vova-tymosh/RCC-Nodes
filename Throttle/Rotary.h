#pragma once
#include <SimpleKalmanFilter.h>

class Rotary
{
private:
    const int VCC_PIN = 7;
    const int IN_PIN = A1;
    const float TO_VALTAGE = 3.3 / 3.6 * 4096;
    SimpleKalmanFilter filter;

public:
    Rotary() : filter(1, 1, 0.2) {};

    void setup()
    {
        pinMode(IN_PIN, INPUT);
        pinMode(VCC_PIN, OUTPUT);
        digitalWrite(VCC_PIN, HIGH);
    }

    int read()
    {
        float a = analogRead(IN_PIN) / TO_VALTAGE * 100;
        // Serial.println("Rotary " + String(a));
        int smoothed = round(filter.updateEstimate(a));
        smoothed = constrain(smoothed, 0, 100);
        if (smoothed <= 2)
            smoothed = 0;
        return smoothed;
    }
};

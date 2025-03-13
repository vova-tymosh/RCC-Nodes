/*
 * RCC Example Locomotive
 *
 *
 */

#include "Peripheral.h"
#include "Motherboard.h"
#include "RCCLoco.h"
#include "Storage.h"
#include "Timer.h"


Storage storage;
Settings settings;
PinExt yellow(2);
Pin blue(D0);
PowerMeter powerMeter;
Motor motor(PIN_MOTOR_BCK, PIN_MOTOR_FWD);
Timer timer;
Timer blinker(1000);



class TestLoco : public RCCLoco
{
public:
    using RCCLoco::RCCLoco;

    void onFunction(char code, bool value)
    {
        // Serial.println("onFunction: " + String((int)code) + "/" + String(value));
        if (code == 0)
            yellow.apply(value);
        if (code == 1)
            blue.apply(value);
    }

    void onThrottle(uint8_t direction, uint8_t throttle)
    {
        motor.apply(direction, throttle);
    }

    void onCommand(uint8_t code, float value)
    {
        switch (code) {
        case 'V':
            Serial.println(powerMeter.readVoltage());
            break;
        case 'C':
            Serial.println(powerMeter.readCurrent());
            break;
        case 'B':
            Serial.println(motor.readBemf());
            break;
        }
    }
};
TestLoco loco;

void setup()
{
    Serial.begin(115200);
    delay(250);

    storage.begin();
    settings.checkDefaults(defaultSettings, defaultSettingsSize);

    motor.setup();
    yellow.begin();
    blue.begin();
    powerMeter.setup();
    timer.start(100);
    blinker.restart();
    
    loco.setup();
}

void loop()
{
    loco.loop();

    if (blinker.hasFired()) {
        static bool flip = false;
        flip = !flip;
        if (flip) {
            digitalWrite(LED_BUILTIN, HIGH);
            // Serial.println("Loco:");
        } else {
            digitalWrite(LED_BUILTIN, LOW);
        }
    }
}


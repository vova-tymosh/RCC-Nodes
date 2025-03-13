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
};
TestLoco loco;


/*
bool on_radio = false;
void (*reboot)(void) = 0;

class Cli : public RccCli
{
public:
    void onExe(uint8_t code)
    {
        switch (code) {
        case '0':
            reboot();
            break;
        case 'V':
            Serial.println(powerMeter.readVoltage());
            break;
        case 'C':
            Serial.println(powerMeter.readCurrent());
            break;
        case 'B':
            Serial.println(motor.readBemf());
            break;
        case 'R':
            Serial.println("Start radio");
            on_radio = true;
            // loco.setup();
            break;
        }
    }

    void onSpeed(uint8_t direction, uint8_t throttle, bool is_dcc)
    {
        Serial.println("onThrottle: " + String(direction) + "/" + String(throttle));
        motor.apply(direction, throttle);

        // if (!is_dcc)
        //     loco.onThrottle(direction, throttle);
        // Serial.println("onSpeed " + String(direction) + "/" + String(throttle));
    }

    void onFunction(uint8_t code, bool value, bool is_dcc)
    {
        if (code == 0)
            yellow.apply(value);
        if (code == 1)
            blue.apply(value);
        // if (!is_dcc)
        //     loco.onFunction(code, value);
        // Serial.println("onFunction " + String(code) + "/" + String(value));
    }
};
Cli */

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
    
    // storage.setup(VERSION);
    loco.setup();
    // loco.debug = true;
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

    // if (on_radio)
    //     loco.loop();
}


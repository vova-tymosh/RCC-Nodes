/*
 * RCC Example Locomotive
 *
 *
 */

#include "Peripheral.h"
// #include "RCCLoco.h"
// #include "SpeedSensor.h"
#include "Timer.h"
#include "Cli.h"




// const int NODE = 01;
// const char *NAME = "t001";
// // Storage storage;
// Wireless wireless(CE_PIN, CSN_PIN);

Timer timer;
Timer blinker(1000);

PinExt yellow(2);
Pin blue(D0);
PowerMeter powerMeter;
Motor2 motor;


/*
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
TestLoco loco(&wireless, NODE, NAME, NULL);//&storage);
*/

bool on_radio = false;
void (*reboot)(void) = 0;

class Cli : public CliBase
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
Cli cli;


void setupSerial()
{
    Serial.begin(115200);
    delay(250);
    Serial.println("Started");
}

void setup()
{
    setupSerial();
    // pinMode(LED_BUILTIN, OUTPUT);

    motor.setup();
    yellow.begin();
    blue.begin();
    powerMeter.setup();
    timer.start(100);
    blinker.restart();
    
    // storage.setup(VERSION);
    // loco.setup();
    // loco.debug = true;
}

void loop()
{
    cli.process();
    
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

    // if (timer.hasFired()) {
    //     loco.state.speed = 0;
    //     loco.state.distance = 0;
    //     loco.state.battery = 71;
    //     loco.state.temperature = 83;
    //     loco.state.psi = 24;
    //     if (loco.debug) {
    //         Serial.println("Loco:" + String(loco.state.throttle) + " " +
    //                     String(loco.state.direction) + " " +
    //                     String(loco.state.speed) + " " +
    //                     String(loco.state.distance));
    //     }
    // }
}


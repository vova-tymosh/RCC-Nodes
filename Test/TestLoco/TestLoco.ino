/*
 * RCC Example Locomotive
 *
 *
 */
#include "Peripheral.h"
#include "RCCLoco.h"
#include "SpeedSensor.h"
#include "Timer.h"

#if defined(ARDUINO_AVR_NANO)
    #define CE_PIN 10
    #define CSN_PIN 9
#elif defined(ARDUINO_AVR_LEONARDO)
    #define CE_PIN 19
    #define CSN_PIN 18
#elif defined(ARDUINO_ARCH_NRF52)
    #define CE_PIN 0
    #define CSN_PIN 0
#endif

const int NODE = 01;
const char *NAME = "t001";
// Storage storage;
Wireless wireless(CE_PIN, CSN_PIN);
Timer timer;
Timer blinker(1000);

Motor motor(7, 8);
Light yellow(0);
Light blue(9);

class TestLoco : public RCCLoco
{
public:
    using RCCLoco::RCCLoco;

    void onFunction(char code, bool value)
    {
        Serial.println("onFunction: " + String((int)code) + "/" + String(value));
        if (code == 0)
            yellow.apply(value);
        if (code == 1)
            blue.apply(value);
    }

    void onThrottle(uint8_t direction, uint8_t throttle)
    {
        // Serial.println("onThrottle: " + String(direction) + "/" + String(throttle));
        motor.apply(direction, throttle);
    }
};
TestLoco loco(&wireless, NODE, NAME, NULL);//&storage);



void setupSerial()
{
    Serial.begin(115200);
    delay(250);
    Serial.println("Started");
}

void setup()
{
    setupSerial();
    pinMode(LED_BUILTIN, OUTPUT);


    // storage.setup(VERSION);
    motor.setup();
    yellow.setup();
    blue.setup();
    loco.setup();
    timer.start(100);
    blinker.restart();

    // loco.debug = true;
}

void loop()
{
    if (blinker.hasFired()) {
        static bool flip = false;
        flip = !flip;
        if (flip) {
            digitalWrite(LED_BUILTIN, HIGH);
        } else {
            digitalWrite(LED_BUILTIN, LOW);
        }
    }


    loco.loop();

    if (timer.hasFired()) {
        loco.state.speed = 0;
        loco.state.distance = 0;
        loco.state.battery = 71;
        loco.state.temperature = 83;
        loco.state.psi = 24;
        if (loco.debug) {
            Serial.println("Loco:" + String(loco.state.throttle) + " " +
                        String(loco.state.direction) + " " +
                        String(loco.state.speed) + " " +
                        String(loco.state.distance));
        }
    }
}

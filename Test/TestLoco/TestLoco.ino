/*
 * RCC Example Locomotive
 *
 *
 */
#include "Peripheral.h"
#include "RCCLoco.h"
#include "SpeedSensor.h"
#include "Timer.h"
#include "Cli.h"

#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

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
        // Serial.println("onFunction: " + String((int)code) + "/" + String(value));
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


class Cli : public CliBase
{
public:
    void onRead()
    {
        Serial.println("read");
    }

    void onSpeed(uint8_t direction, uint8_t throttle, bool is_dcc)
    {
        if (!is_dcc)
            loco.onThrottle(direction, throttle);
        // Serial.println("onSpeed " + String(direction) + "/" + String(throttle));
    }

    void onFunction(uint8_t code, bool value, bool is_dcc)
    {
        if (!is_dcc)
            loco.onFunction(code, value);
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


void power()
{
    float shuntvoltage = 0;
    float busvoltage = 0;
    float current_mA = 0;
    float loadvoltage = 0;
    float power_mW = 0;

    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    // As shunt is 0.1
    current_mA = ina219.getCurrent_mA() * 10;
    power_mW = ina219.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);

    Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
    Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
    Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
    Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
    Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
    Serial.println("");
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

    if (! ina219.begin()) {
        Serial.println("Failed to find INA219 chip");
    }

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
            power();
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

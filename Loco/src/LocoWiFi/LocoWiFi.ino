/*
 * Copyright (c) 2024-2025 Volodymyr "Vova" Tymoshchuk
 * Distributed under MIT licence, https://github.com/vova-tymosh/RCC/blob/main/LICENSE
 * For more details go to https://github.com/vova-tymosh/RCC
 *
 * The above copyright notice shall be included in all
 * copies or substantial portions of the Software.
 */
#include "Peripheral.h"
#include "SpeedSensor.h"
#include "Timer.h"
#include "Settings.h"
#include "Motherboard.h"
#include "RCCLoco.h"


Storage storage;
Settings settings;
PowerMeter powerMeter;
Timer timer;
const float inches_per_click = 0.245;
SpeedSensor speedSensor(PIN_SPEED_PIN, inches_per_click);

Motor motor(PIN_MOTOR_BCK, PIN_MOTOR_FWD);
const int MAX_FUNCTION = 6;
PinExt function[MAX_FUNCTION] = {PinExt(0), PinExt(1), PinExt(2), PinExt(3), PinExt(4), PinExt(5)};

const int settingsSize = 12;
const char *settingsKeys[settingsSize] = {
    "wifiap",       "wifissid",   "wifipwd",      "loconame",    "locoaddr",
    "broker",       "brokerport", "acceleration", "managespeed", "heartbeat", 
    "mqtt",       "pump"};
const char *settingsValues[settingsSize] = {
    "ON",           "RCC_Loco",   "RCC_Loco",     "RCC",         "3",
    "192.168.0.10", "1883",       "0",            "0",           "1000",
    "ON",           "0"};


class SimpleWiThrottleLoco : public RCCLoco
{
public:
    using RCCLoco::RCCLoco;

    void onFunction(uint8_t code, bool activate)
    {
        Serial.print("Function: "); Serial.print(code); Serial.println( activate ? " ON" : " OFF");
        code = constrain(code, 0, 5);
        function[code].apply(activate);
    }

    void onThrottle(uint8_t direction, uint8_t throttle)
    {
        Serial.print("Throttle: "); Serial.print(throttle); Serial.print(" D:"); Serial.println(direction); 
        motor.apply(direction, throttle);
    }
};
SimpleWiThrottleLoco loco;

void setup()
{
    Serial.begin(115200);
    delay(50);
    storage.begin();
    settings.begin(settingsKeys, settingsValues, settingsSize);
    loco.begin();
    speedSensor.begin();
    powerMeter.begin();
    motor.begin();
    for (int i = 0; i < MAX_FUNCTION; i++)
        function[i].begin();
    timer.start(100);
}

void loop()
{
    loco.loop();
    speedSensor.loop();

    if (timer.hasFired()) {
        loco.state.distance = speedSensor.getDistance();
        loco.state.speed = speedSensor.getSpeed();
        loco.state.battery = powerMeter.readBattery();
        loco.state.current = powerMeter.readCurrent();
    }}
 

#define RCC_NO_STATION
#include "RCCLoco.h"
#include "Peripheral.h"
#include "SpeedSensor.h"
#include "Timer.h"
#include "Settings.h"
#include "Motherboard.h"


Storage storage;
Settings settings;
Timer timer;
const float inches_per_click = 0.245;
SpeedSensor speed_sensor(PIN_SPEED_PIN, inches_per_click);
SpeedSensorBase *speed_sensor_ptr = &speed_sensor;

Motor motor;
PinExt function[] = {PinExt(0), PinExt(1), PinExt(2), PinExt(3), PinExt(4), PinExt(5)};


class SimpleWiThrottleLoco : public RCCLoco
{
public:
    using RCCLoco::RCCLoco;

    void onFunction(uint8_t code, bool activate)
    {
        Serial.print("Function: ");
        Serial.print(code);
        Serial.print(" ");
        if (activate)
            Serial.println("ON");
        else
            Serial.println("OFF");
        code = constrain(code, 0, 5);
        function[code].apply(activate);
    }

    void onThrottle(uint8_t direction, uint8_t throttle)
    {
        Serial.print("Throttle: ");
        if (direction)
            Serial.print("FWD ");
        else
            Serial.print("REV ");
        Serial.println(throttle);
        motor.apply(direction, throttle);
    }
};
SimpleWiThrottleLoco loco;

void setup()
{
    Serial.begin(115200);
    delay(100);
    storage.begin();
    settings.checkDefaults(defaultSettings, defaultSettingsSize);
    loco.setup();
    speed_sensor.setup();
    motor.setup();
    function[0].begin();

    timer.start(500);
}

void loop()
{
    loco.loop();
    speed_sensor.loop();
    motor.loop();

    if (timer.hasFired()) {
        loco.state.speed = speed_sensor.getSpeed();
        loco.state.distance = speed_sensor.getDistance();
        String name = settings.get("loconame");
        Serial.println(name + ": " + String(loco.state.throttle)   + " " +
                            String(loco.state.direction)
                              + " " + String(loco.state.speed)   + " " +
                              String(loco.state.distance));
    }
}
 

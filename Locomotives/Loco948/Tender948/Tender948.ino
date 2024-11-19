#include "Intercom.h"
#include "Peripheral.h"
#include "RCCLoco.h"
#include "SpeedSensor.h"
#include "Timer.h"

#include "DCC_Controller.h"

/*

PID Settings
 P 1    0.5
 I 0
 D 0
 M 2000 25


*/

// *** Comms
const int NODE = 03;
const char *NAME = "948";
Wireless wireless(19, 18);
Storage storage;

// *** Periferals
Timer timer;
SpeedSensor speed_sensor(7, 0.190);
SpeedSensorBase *speed_sensor_ptr = &speed_sensor;

// *** Watchdog
Timer watchdog;
void (*reboot)(void) = 0;

// *** Intercom
struct {
    uint8_t direction;
    uint8_t throttle;
    uint8_t lights;
} controls;

class Tender948 : public RCCLoco
{
    uint8_t _direction;
    uint8_t _throttle;

public:
    using RCCLoco::RCCLoco;

    void onFunction(char code, bool value)
    {
        dcc.cmdFunction(code, value);
    }

    void onThrottle(uint8_t direction, uint8_t throttle)
    {
        if ((_direction != direction) || (_throttle != throttle)) {
            uint8_t d = (direction == 2) ? 0 : 1;
            uint8_t t = map(throttle, 0, 100, 0, 128);
            dcc.cmdSpeed(d, t);
            _direction = direction;
            _throttle = throttle;
        }
    }

    // void onCommand(char code, float value) {
    //   if (code == 'f') {
    //     Serial.println("PID:" + String(pid.p) + " " + String(pid.i)
    //                     + " " + String(pid.d) + " " + String(pid.upper));
    //   }
    // }
};
Tender948 loco(&wireless, NODE, NAME, &storage);

void setup()
{
    Serial.begin(115200);
    speed_sensor.setup();
    loco.setup();
    timer.start(100);
    watchdog.start(5000);

    dcc.setup();
}

void loop()
{
    loco.loop();
    speed_sensor.loop();
    dcc.processCLI();

    if (timer.hasFired()) {
        loco.state.speed = speed_sensor.getSpeed();
        loco.state.distance = speed_sensor.getDistance();
        loco.state.battery = 0;
        // Serial.println("Tender:" + String(loco.state.throttle) + " " +
        // String(loco.state.direction)
        //                  + " " + String(loco.state.speed) + " " +
        //                  String(loco.state.distance)
        //                  + " " + String(loco.pid.p) + " " +
        //                  String(loco.pid.i) + " " + String(loco.pid.d) + " "
        //                  + String(loco.pid.upper)
        //                  );
    }

    if (watchdog.hasFired()) {
        if (!loco.isTransmitting()) {
            Serial.println("Radio watchdog fired, reboot");
            delay(100);
            reboot();
        }
    }
}

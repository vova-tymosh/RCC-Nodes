#include "Timer.h"
#include "SpeedSensor.h"
#include "Peripheral.h"
#include "RCCLoco.h"
#include "Intercom.h"



/*

PID Settings
 P 1    0.5
 I 0
 D 0
 M 2000 25


*/


// *** Comms
const int   NODE = 03;
const char *NAME = "948";
Wireless wireless(19, 18);
Storage storage;

// *** Periferals
Timer timer;
SpeedSensor speedSensor(7, 0.190);
SpeedSensorBase *speedSensorPtr = &speedSensor;

// *** Watchdog
Timer watchdog;
void(* reboot) (void) = 0;

// *** Intercom
struct {
  uint8_t direction;
  uint8_t throttle;
  uint8_t lights;
} controls;

class Tender948: public RCCLoco {
  public:
    using RCCLoco::RCCLoco;

    void onFunction(char code, bool value) {
      controls.lights = state.lights;
    }

    void onThrottle(uint8_t direction, uint8_t throttle) {
      controls.direction = direction;
      controls.throttle = throttle;
    }

    void onCommand(char code, float value) {
      if (code == 'f') {
        Serial.println("PID:" + String(pid.p) + " " + String(pid.i)
                        + " " + String(pid.d) + " " + String(pid.upper));

      }
    }
};
Tender948 loco(&wireless, NODE, NAME, &storage);

void setup() {
  Serial.begin(115200);
  speedSensor.setup();
  intercom.setup(true);
  loco.setup();
  timer.start(100);
  watchdog.start(5000);
}

void loop() {
  loco.loop();
  speedSensor.loop();

  if (timer.hasFired()) {
    // intercom.recv(&sensors, sizeof(sensors));
    intercom.send(&controls, sizeof(controls));

    loco.state.speed = speedSensor.getSpeed();
    loco.state.distance = speedSensor.getDistance();
    loco.state.battery = 0;
    // Serial.println("Tender:" + String(loco.state.throttle) + " " + String(loco.state.direction)
    //                  + " " + String(loco.state.speed) + " " + String(loco.state.distance)
    //                  + " " + String(loco.pid.p) + " " + String(loco.pid.i) + " " + String(loco.pid.d) + " " + String(loco.pid.upper)
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
 
#include "Timer.h"
#include "SpeedSensor.h"
#include "Peripheral.h"
#include "RCCLoco.h"
#include "Intercom.h"

#include "DCC_Controller.h"


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
  uint8_t _direction;
  uint8_t _throttle;
  public:
    using RCCLoco::RCCLoco;

    void onFunction(char code, bool value) {
      char line[10];
      if (value) value = 1;
      sprintf(line, "F3%02d%1d", code, value);
      bool result = DCC.processCommand(line);
      Serial.print(line);
      Serial.println(result? " ok" : " error");
    }

    void onThrottle(uint8_t direction, uint8_t throttle) {
      if ((_direction != direction) || (_throttle != throttle)) {
        uint8_t t = map(throttle, 0, 100, 0, 128);
        char line[10];
        if (direction == 2)
          sprintf(line, "S30%03d", throttle);
        else
          sprintf(line, "S31%03d", throttle);
        bool result = DCC.processCommand(line);
        Serial.print(line);
        Serial.println(result? " ok" : " error");
      _throttle = throttle;
        _direction = direction;
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


#define INPUT_LEN_MAX 10
bool isStarted = false;
char inString[INPUT_LEN_MAX];
uint8_t inPos = 0;
bool result;


void setup() {
  Serial.begin(115200);
  speedSensor.setup();
  loco.setup();
  timer.start(100);
  watchdog.start(5000);

  DCC.DCC_begin();
}

void loop() {
  while (Serial.available()) {
    char inChar = Serial.read();
    if (inChar == '<'){
      isStarted = true;
      inPos = 0;
    }
    else if (inChar == '>' && isStarted){
      inString[inPos] = '\0';
      result = DCC.processCommand(inString);
      Serial.print(inString);
      Serial.println(result? " ok" : " error");
      isStarted = false;
      inString[0] = '\0';
    }
    else{
      inString[inPos++] = inChar;
      inPos %= INPUT_LEN_MAX;
    }
  }



  loco.loop();
  speedSensor.loop();

  if (timer.hasFired()) {
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

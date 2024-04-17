#include <Keypad.h>
#include "Timer.h"
#include "Rotary.h"
#include "UI.h"
#include "LocoState.h"
#include "HomeScreen.h"
#include "MenuScreen.h"

#define RADIO_52
#include "Wireless.h"
#include "ThrComms.h"

// *** Keyboard
const byte ROWS = 4;
const byte COLS = 3;
const char keys[ROWS][COLS] = {
  {'h','m','d'},
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
};
byte rowPins[ROWS] = {8,9,10,0};
byte colPins[COLS] = {1,6,3};
Keypad keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// *** Comms
const int   locoAddr = 4;
Wireless wireless(locoAddr);
ThrComms comms(&wireless);
struct Loco loco;

// *** Screens and UI
UserInterface ui;
typedef void* (*State)(char key);
State state;
void* homeState(char key);
void* menuState(char key);
HomeScreen homeScreen(&loco);
MenuScreen menuScreen;

// *** Timer and sensors
Timer timer;
Rotary rotary;


void received(char code, float value) {
  Serial.println("Command " + String(code) + ": " + String(value, 2));
}

void handleHotKey(char key) {
  switch (key) {
  case '1':
    if (loco.direction < 1)
      loco.direction++;
    else
      loco.direction = -1;
    break;   
  case '2':
    if (loco.battery < 3)
      loco.battery++;
    else
      loco.battery = 0;
    break;
  case '3':
    if (loco.light)
      loco.light = 0;
    else
      loco.light = 1;
    break;
  }
}

//TODO consider class states
void* homeState(char key) {
  if (key == 'm')
    return (void*)menuState;
  homeScreen.draw();
  return 0;
}

void* menuState(char key) {
  if (key == 'h')
    return (void*)homeState;
  menuScreen.draw(key);
  return 0;
}

//TODO remove
void setupSerial() {
  Serial.begin(115200);
  for (int i=0; i<5 || !Serial; i++);
  Serial.println("Started");
}

const char *nameL = "1204";

void setup() {
  setupSerial();

//TODO remove this and use incoming
  loco.name = (char*)nameL;
  loco.lost = 12;
  loco.battery = 1;
  loco.speed = 101;
  loco.temperature = 121;
  loco.psi = 24;
  loco.odo = 12030;

  comms.setup();
  comms.setCallback(received);

//TODO consider more to make sure Station heard us
  comms.send('a', 0);
  rotary.setup();

  ui.setup();
  state = homeState;
  state(0);

  timer.start(100);
}

void loop() {
  static int oldThrottle = 0;

  bool update = comms.loop();

  char key = keypad.getKey();
  loco.throttle = rotary.read();

  //TODO handle proper updates depands on events
  if (key || (oldThrottle != loco.throttle) || update) {
    if (key) {
      Serial.println("Press " + String(key));
      handleHotKey(key);
      comms.send('l', (float)loco.light);
    } else if (oldThrottle != loco.throttle) {
      oldThrottle = loco.throttle;
      comms.send('t', (float)loco.throttle);
    }

    void *newState = state(key);
    if (newState) {
      state = (State)newState;
      state(0);
    }
  }
}

#include <Keypad.h>
#include "Timer.h"
#include "Rotary.h"
#include "UI.h"
#define RADIO_52
#include "Wireless.h"
#include "ThrComms.h"
#include "LocoState.h"
#include "HomeScreen.h"
#include "MenuScreen.h"


// *** Keyboard
const byte ROWS = 4;
const byte COLS = 3;
const char keys[ROWS][COLS] = {
  {'h','m','2'},
  {'1','d','3'},
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
extern MenuItem *menuItem[];

// *** The rest
Timer timer;
Rotary rotary;
struct Controls controls;



void received(char code, float value) {
  Serial.println("Command " + String(code) + ": " + String(value, 2));
}

void handleHotKey(char key) {
  switch (key) {
  case '1':
    menuItem[0]->toggle();
    break;
  case '2':
    if (loco.battery < 3)
      loco.battery++;
    else
      loco.battery = 0;
    break;
  case '4':
    if (controls.direction < 1)
      controls.direction++;
    else
      controls.direction = -1;
    comms.send('d', (float)controls.direction);
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

void setupSerial() {
  Serial.begin(115200);
  Serial.println("Started");
}

const char *nameL = "1204";

void setup() {
  setupSerial();

  comms.setup();
  comms.setCallback(received);

//TODO consider more to make sure Station heard us
  comms.send('a', 0);
  rotary.setup();

  ui.setup();
  state = homeState;
  state(0);

  timer.start(500);
}

void loop() {
  static int oldThrottle = 0;

  bool update = comms.loop();
  char key = keypad.getKey();

  if (timer.hasFired()) {
    controls.throttle = rotary.read();
  }

  //TODO handle proper updates depands on events
  if (key || (oldThrottle != controls.throttle) || update) {
    if (key) {
      Serial.println("Press " + String(key));
      handleHotKey(key);
    } else if (oldThrottle != controls.throttle) {
      oldThrottle = controls.throttle;
      comms.send('t', (float)controls.throttle);
    }

    void *newState = state(key);
    if (newState) {
      state = (State)newState;
      state(0);
    }
  }
}

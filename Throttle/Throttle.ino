#include <Keypad.h>
#include "Timer.h"
#include "Rotary.h"
#include "UI.h"
#include "Wireless.h"
#include "ThrComms.h"
#include "LocoState.h"
#include "HomeScreen.h"
#include "MenuScreen.h"
#include "Battery.h"
#include "Storage.h"


// *** Keyboard
const byte ROWS = 4;
const byte COLS = 3;
const char keys[ROWS][COLS] = {
  {'h','m','d'},
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
};
byte rowPins[ROWS] = {8,9,10,3};
byte colPins[COLS] = {2,6,0};
Keypad keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// *** Comms
const int node = 4;
Wireless wireless;
ThrComms comms(&wireless);
struct Loco loco;

// *** Screens and UI
UserInterface ui;
HomeScreen homeScreen;
MenuScreen menuScreen;
BaseState *states[] = {(BaseState*)0, &homeScreen, &menuScreen};
BaseState *state;

// *** The rest
Timer screensaver;
Timer timer;
Rotary rotary;
Battery battery;
struct Controls controls;
struct Setting setting;


void handleHotKey(char key) {
  switch (key) {
  case '1':
    menuItem[1]->toggle();
    break;
  case '2':
    if (loco.battery < 3)
      loco.battery++;
    else
      loco.battery = 0;
    break;
  case 'd':
    if (loco.speed == 0) {
      if (controls.direction == 1)
        controls.direction = 2;
      else
        controls.direction = 1;
    } else {
      controls.direction = 0;
    }
    comms.send('d', (float)controls.direction);
    break;
  }
}

void setupSerial() {
  Serial.begin(115200);
  Serial.println("Started");
}

void setup() {
  setupSerial();
  setupFS();

  setting.bitstate = restoreFS();
  Serial.println("Settings: " + String(setting.bitstate));

  if (setting.local)
    comms.setup(0);
  else
    comms.setup(node);
  battery.setup();
  rotary.setup();
  ui.setup();
  menuScreen.setup();


  state = &homeScreen;
  state->handle(0);
  timer.start(250);
  screensaver.start(30000);
}

void loop() {
  static bool powerOn = true;
  bool incoming = comms.loop();
  bool update = false;


  char key = keypad.getKey();
  if (key) {
    Serial.println("Press " + String(key));
    handleHotKey(key);
    update = true;
  }

  if (timer.hasFired()) {
    static int oldThrottle = 0;
    controls.throttle = rotary.read();
    if (oldThrottle != controls.throttle) {
      oldThrottle = controls.throttle;
      comms.send('t', (float)controls.throttle);
    }
    update = true;
  }

  if (update) {
    powerOn = true;
    ui.powerOn();
    screensaver.restart();
  } else if (screensaver.hasFiredOnce()) {
    powerOn = false;
    ui.powerOff();
  }

  if (powerOn && (update || incoming)) {
    controls.lost = comms.getLostRate();

    State newState = state->handle(key);
    if (newState) {
      state = states[newState];
      state->handle(0);
    }
  }
}

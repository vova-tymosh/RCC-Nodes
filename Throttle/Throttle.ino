#define NO_DEBUG
#include "Battery.h"
#include "HomeScreen.h"
#include "LocoState.h"
#include "MenuScreen.h"
#include "Rotary.h"
#include "UI.h"
#include <Keypad.h>

#include "RCCKeypad.h"
#include "Storage.h"
#include "Settings.h"
#include "Timer.h"

// *** Keyboard
const byte ROWS = 4;
const byte COLS = 3;
const char keyMap[ROWS][COLS] = {
    {'h', 'm', 'd'},
    {'1', '2', '3'},
    {'4', '5', '6'},
};
byte row_pins[ROWS] = {8, 9, 10};
byte col_pins[COLS] = {2, 6, 7};
Keypad keys(makeKeymap(keyMap), row_pins, col_pins, ROWS, COLS);

// *** Screens and UI
UserInterface ui;
HomeScreen home_screen;
MenuScreen menu_screen;
BaseState *states[] = {(BaseState *)0, &home_screen, &menu_screen};
BaseState *state;

// *** The rest
Timer screensaver;
Timer rotary_timer;
Timer vsync;
Rotary rotary;
Battery battery;
struct Controls controls;
Storage storage;
Settings settings;

Pad pad;


void toggleFunction(int index)
{
    pad.setFunction(index, ((pad.state.bitstate & (1 << index)) ? 0 : 1));
}

void handleHotKey(char key)
{
    switch (key) {
    case '1':
        menuItem[1]->toggle();
        break;
    case '3':
        controls.timerBase = millis();
        break;
    case '4':
        toggleFunction(1);
        break;
    case '5':
        toggleFunction(3);
        break;
    case '6':
        toggleFunction(23);
        break;
    }
}

void setupSerial()
{
    Serial.begin(115200);
    Serial.println("Started");
}

const char *padKeys[] = {"loconame", "locoaddr", "BigFont"};
const char *padValues[] = {"RCC_Keypad", "1", "0"};
const int padKeySize = sizeof(padKeys) / sizeof(padKeys[0]);


void setup()
{
    setupSerial();
    storage.begin();

    settings.begin(padKeys, padValues, padKeySize);

    pad.debugLevel = 10;
    pad.begin();

    battery.setup();
    rotary.setup();
    ui.setup();
    menu_screen.setup();

    state = &home_screen;
    state->handle(0);
    rotary_timer.start(100);
    vsync.start(100);

    screensaver.start(3 * 60 * 1000);
}

void loop()
{
    static bool powerOn = true;
    bool update = false;
    bool wake = false;

    pad.loop();
    if (pad.update) {
        update = true;
        controls.direction = pad.state.direction;
    }

    char key = keys.getKey();
    if (key) {
        Serial.println("Press " + String(key));
        handleHotKey(key);
        wake = true;
        update = true;
    }

    if (rotary_timer.hasFired()) {
        static int oldThrottle = 0;
        controls.throttle = rotary.read();
        if (oldThrottle != controls.throttle) {
            oldThrottle = controls.throttle;
            pad.setThrottle(controls.throttle);
            // pad.askHeartbeat();
            update = true;
        }
    }

    if (vsync.hasFired()) {
        update = true;
    }

    if (wake) {
        powerOn = true;
        ui.powerOn();
        screensaver.start();
        update = true;
    } else if (screensaver.hasFiredOnce()) {
        powerOn = false;
        ui.powerOff();
    }

    if (powerOn && update) {
        State newState = state->handle(key);
        if (newState) {
            state = states[newState];
            state->handle(0);
        }
    }
}

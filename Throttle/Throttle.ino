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
    {'h', 'b', 'm'},
    {'l', 's', 'd'},
    {'1', '2', '3'},
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
Timer vsync;
Timer throttleUpdate;
Rotary rotary(D1, D0);
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
    case 'l':
        // 2nd menu item - lights
        menuItem[1]->toggle();
        break;
    case 's':
        // controls.direction = 2;
        controls.throttle = 0;
        break;
    case 'b':
        // controls.timerBase = millis();
        // 1st menu item - loco (move to next one)
        menuItem[0]->toggle();
        throttleUpdate.start(500);
        break;
    case '1':
        toggleFunction(1);
        break;
    case '2':
        toggleFunction(3);
        break;
    case '3':
        toggleFunction(23);
        break;
    }
}

const char *padKeys[] = {"loconame", "locoaddr", "BigFont"};
const char *padValues[] = {"RCC_Keypad", "1", "0"};
const int padKeySize = sizeof(padKeys) / sizeof(padKeys[0]);

void setup()
{
    Serial.begin(115200);
    Serial.println("Started");

    storage.begin();

    settings.begin(padKeys, padValues, padKeySize);

    pad.begin();

    battery.setup();
    ui.setup();
    menu_screen.setup();

    state = &home_screen;
    state->handle(ON_ENTER);
    vsync.start(100);
    screensaver.start(3 * 60 * 1000);

    rotary.begin();
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

    if (throttleUpdate.hasFiredOnce()) {
        key = ON_ENTER;
        update = true;
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
            state->handle(ON_ENTER);
        }
    }
}

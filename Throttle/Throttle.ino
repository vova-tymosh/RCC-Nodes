#include "Battery.h"
#include "HomeScreen.h"
#include "LocoState.h"
#include "MenuScreen.h"
#include "Rotary.h"
#include "Storage.h"
#include "ThrComms.h"
#include "Timer.h"
#include "UI.h"
// #include "Wireless.h"
#include <Keypad.h>

// *** Keyboard
const byte ROWS = 4;
const byte COLS = 3;
const char keys[ROWS][COLS] = {
    {'h', 'm', 'd'},
    {'1', '2', '3'},
    {'4', '5', '6'},
};
byte row_pins[ROWS] = {8, 9, 10};
byte col_pins[COLS] = {2, 6, 7};
Keypad keypad(makeKeymap(keys), row_pins, col_pins, ROWS, COLS);

// *** Comms
const int node = 4;
// Wireless wireless;
ThrComms comms;
struct LocoState loco;

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
Storage storage;
struct Controls controls;
struct Setting setting;

void toggleFunction(int index)
{
    comms.sendFunction(index, ((loco.bitstate & (1 << index)) ? 0 : 1));
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

void setup()
{
    setupSerial();
    storage.begin();

    // setting.bitstate = storage.restore();
    storage.readOrCreate("setting", &setting.bitstate, sizeof(setting.bitstate));

    Serial.println("Settings: " + String(setting.bitstate));

    if (setting.local)
        comms.setup(0);
    else
        comms.setup(node);
    battery.setup();
    rotary.setup();
    ui.setup();
    menu_screen.setup();

    state = &home_screen;
    state->handle(0);
    rotary_timer.start(100);
    vsync.start(250);
    screensaver.start(3 * 60 * 1000);
}

void loop()
{
    static bool powerOn = true;
    bool update = false;
    bool wake = false;

    if (comms.loop()) {
        update = true;
        controls.direction = loco.direction;
    }

    char key = keypad.getKey();
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
            comms.send('t', (float)controls.throttle);
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
        controls.lost = comms.getLostRate();

        State newState = state->handle(key);
        if (newState) {
            state = states[newState];
            state->handle(0);
        }
    }
}

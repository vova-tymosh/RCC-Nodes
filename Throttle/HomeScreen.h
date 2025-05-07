#pragma once

#include "Battery.h"
#include "LocoState.h"
#include "States.h"
#include "Timer.h"
#include "UI.h"
// #include "ThrComms.h"

extern UserInterface ui;
// extern ThrComms comms;

class HomeScreen : public BaseState
{
private:
    Timer battery_cycle;

    char renderDirection()
    {
        if (keypad.state.direction == 0)
            return 'R';
        else
            return 'F';
    }

    void renderBattery()
    {
        static int animation;
        int level = battery.getLevel();
        if (battery.isCharging()) {
            if (battery_cycle.hasFired()) {
                if (animation >= UserInterface::BATTERY_FULL)
                    animation = 0;
                else
                    animation++;
            }
            level = animation;
        }
        ui.showBattery((UserInterface::BatteryState)level);
    }

    void renderDisatnce(char *line, int distance)
    {
        if (distance < 1000) {
            sprintf(line, "%-3d          ", distance);
        } else if (distance < 10000) {
            sprintf(line, "%.2fk         ", (float)distance / 1000);
        } else {
            sprintf(line, "%.1fk         ", (float)distance / 1000);
        }
    }

    void renderTime(char *line)
    {
        unsigned long now = millis();
        now = now - controls.timerBase;
        unsigned long sec = now / 1000;
        unsigned long min = sec / 60;
        sec = sec % 60;
        sprintf(line, "%02d:%02d", min, sec);
    }

    void renderSmall()
    {
        static const char fmt1[] = "Loco:%4s   %3d%%";
        static const char fmt2[] = "T:%-3d R:%-3d S:%-3d D:%c";
        static const char fmt3[] = "%2d%cC    %2dpsi    ";
        static const char fmt4[] = "ODO:                     ";
        char line[ui.width + 1];

            //TODO
            Serial.println("---------------");

        ui.startScreen();
        unsigned int lost = 0 + controls.lost;
        if (lost > 100)
            lost = 100;
        //TODO
        sprintf(line, fmt1, keypad.transport->getSelectedName(), lost);
        ui.display.println(line);
        sprintf(line, fmt2, controls.throttle, keypad.state.throttle, keypad.state.speed, renderDirection());
            //TODO
            Serial.println(line);
        ui.display.println(line);
        sprintf(line, fmt3, keypad.state.temperature, 0xF7, keypad.state.psi);
            //TODO
            Serial.println(line);
        ui.display.println(line);
        sprintf(line, fmt4);
        renderDisatnce(line + 4, keypad.state.distance);
        renderTime(line + 16);
            //TODO
            Serial.println(line);
        ui.display.println(line);

        renderBattery();
        ui.display.display();
    }

    void renderBig()
    {
        static const char fmt1[] = "T:%-3d %c%c";
        static const char fmt2[] = "%2d%c %.1fk";
        char line[ui.width + 1];

        ui.startScreen();
        ui.display.setTextSize(2);
        // char alive = comms.isAlive() ? 0x1F : ' ';
        // sprintf(line, fmt1, controls.throttle, renderDirection(), alive);
        ui.display.println(line);
        sprintf(line, fmt2, keypad.state.temperature, 0xF7,
                (float)keypad.state.distance / 1000);
        ui.display.println(line);

        renderBattery();
        ui.display.setTextSize(1);
        ui.display.display();
    }

public:
    HomeScreen() : battery_cycle(500) {};

    State handle(char key)
    {
        if (key == 'm') {
            return STATE_MENU;
        } else if (key == 'd') {
            if (controls.direction != 0)
                controls.direction = 0;
            else
                controls.direction = 1;
            // comms.send('d', (float)controls.direction);
        }
        if (setting.bigui)
            renderBig();
        else
            renderSmall();
        return STATE_NONE;
    }
};

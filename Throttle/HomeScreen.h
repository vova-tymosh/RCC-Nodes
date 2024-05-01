#pragma once

#include "LocoState.h"
#include "States.h"
#include "Battery.h"
#include "Timer.h"


extern UserInterface ui;
extern ThrComms comms;

class HomeScreen : public BaseState {
  private:
    Timer cycle;
    char renderDirection() {
      if (loco.direction == 2)
        return 'R';
      else if (loco.direction == 1)
        return 'F';
      else
        return '|';
    }
    void renderBattery() {
      static int animation;
      int level = battery.getLevel();
      if (battery.isCharging()) {
        if (cycle.hasFired()) {
          if (animation >= UserInterface::BATTERY_FULL)
            animation = 0;
          else
            animation++;
        }
        level = animation;
      }
      ui.showBattery((UserInterface::BatteryState)level);
    }

  public:
    HomeScreen(): cycle(500) {};

    State handle(char key) {
      if (key == 'm')
        return STATE_MENU;

      static const char fmt1[] = "Loco:%s %d%%";
      static const char fmt2[] = "T:%-3d R:%-3d S:%-3d D:%c";
      static const char fmt3[] = "%d%cC     %dpsi";
      static const char fmt4[] = "ODO:%d %d";
      char line[ui.width + 1];

      ui.startScreen();
      unsigned int lost = loco.lost + controls.lost;
      if (lost > 100)
        lost = 100;
      sprintf(line, fmt1, comms.availableLocos[comms.selectedLoco].name, lost);
      ui.display.println(line);
      sprintf(line, fmt2, controls.throttle, loco.throttle, loco.speed, renderDirection());
      ui.display.println(line);
      sprintf(line, fmt3, loco.temperature, 247, loco.psi);
      ui.display.println(line);
      // sprintf(line, fmt4, loco.disatnce);
      int v = battery.getVoltage() * 100;
      sprintf(line, fmt4, v, battery.isCharging());
      ui.display.println(line);
      
      renderBattery();
      ui.display.display();
      return STATE_NONE;
    }
};

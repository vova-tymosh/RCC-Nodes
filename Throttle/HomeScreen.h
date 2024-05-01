#pragma once

#include "LocoState.h"
#include "States.h"

extern UserInterface ui;
extern ThrComms comms;

class HomeScreen : public BaseState {
  private:
    char renderDirection() {
      if (loco.direction == 2)
        return 'R';
      else if (loco.direction == 1)
        return 'F';
      else
        return '|';
    }
    void renderBattery(uint16_t battery) {
      battery = 3 * battery / 100;
      if (battery < UserInterface::BATTERY_FULL)
        ui.showBattery((UserInterface::BatteryState)battery);
    }

  public:
    State handle(char key) {
      if (key == 'm')
        return STATE_MENU;

      static const char fmt1[] = "Loco:%s %d%%";
      static const char fmt2[] = "T:%-3d R:%-3d S:%-3d D:%c";
      static const char fmt3[] = "%d%cC     %dpsi";
      static const char fmt4[] = "ODO:%d";
      char line[ui.width + 1];

      ui.startScreen();
      sprintf(line, fmt1, comms.availableLocos[comms.selectedLoco].name, loco.lost + controls.lost);
      ui.display.println(line);
      sprintf(line, fmt2, controls.throttle, loco.throttle, loco.speed, renderDirection());
      ui.display.println(line);
      sprintf(line, fmt3, loco.temperature, 247, loco.psi);
      ui.display.println(line);
      sprintf(line, fmt4, loco.disatnce);
      ui.display.println(line);
      
      renderBattery(loco.battery);
      ui.display.display();
      return STATE_NONE;
    }
};

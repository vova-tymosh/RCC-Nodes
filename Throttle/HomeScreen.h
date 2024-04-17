#pragma once

#include "LocoState.h"

extern UserInterface ui;

class HomeScreen {
  private:
    struct Loco *loco;
    
    char renderDirection() {
      if (loco->direction == -1)
        return 'R';
      else if (loco->direction == 1)
        return 'F';
      else
        return '|';      
    }
    
  public:
    HomeScreen(struct Loco *loco) : loco(loco) {};
    
    void draw() {
      static const char fmt1[] = "Loco:%s   %d%%";
      static const char fmt2[] = "T:%-3d    S:%-3d    D:%c";
      static const char fmt3[] = "%d%cC     %dpsi";
      static const char fmt4[] = "ODO:%d";
      char line[ui.width + 1];

      ui.startScreen();
      sprintf(line, fmt1, loco->name, loco->lost);
      ui.display.println(line);
      sprintf(line, fmt2, loco->throttle, loco->speed, renderDirection());
      ui.display.println(line);
      sprintf(line, fmt3, loco->temperature, 247, loco->psi);
      ui.display.println(line);
      sprintf(line, fmt4, loco->odo);
      ui.display.println(line);
      
      ui.showBattery((UserInterface::BatteryState)loco->battery);
      ui.display.display();
    }
};

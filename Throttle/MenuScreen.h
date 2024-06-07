#pragma once

#include "UI.h"
#include "ThrComms.h"
#include "States.h"
#include "Storage.h"

extern UserInterface ui;
extern ThrComms comms;

class MenuItem {
  protected:
    const char *name;
    const int index;

  public:
    static const int LINES = 4;
    MenuItem(const char *name, const int index): name(name), index(index) {}
    virtual void setup() {};
    virtual void render(char *line, size_t size) = 0;
    virtual void toggle() = 0;
};

class MenuItemToggle: public MenuItem {
  public:
    MenuItemToggle(const char *name, const int index): MenuItem(name, index) {}

    void render(char *line, size_t size) {
      static const char fmt1[] = "%-18s%s";
      if (loco.bitstate & (1 << index))
        snprintf(line, size, fmt1, name, "ON");
      else
        snprintf(line, size, fmt1, name, "OFF");        
    }

    void toggle() {
      int state = (loco.bitstate & (1 << index));
      if (state)
        state = 0;
      else
        state = 1;
      comms.sendFunction(index, state);
    }
};

class MenuItemToggleConfig: public MenuItemToggle {
    int savedState;
  public:
    MenuItemToggleConfig(const char *name, const int index): MenuItemToggle(name, index) {}

    virtual const char *getName(String &title) {
      return name;
    }

    void render(char *line, size_t size) {
      static const char fmt1[] = "%-18s%s";
      String title;
      if (setting.bitstate & (1 << index))
        snprintf(line, size, fmt1, getName(title), "ON");
      else
        snprintf(line, size, fmt1, getName(title), "OFF");
    }

    void toggle() {
      setting.bitstate ^= (1 << index);
      storage.save(setting.bitstate);
    }
};

class MenuItemToggleLocal: public MenuItemToggleConfig {
    int savedState;
  public:
    MenuItemToggleLocal(const char *name, const int index): MenuItemToggleConfig(name, index) {}

    void setup() {
      savedState = setting.bitstate & (1 << index);
    }

    const char *getName(String &title) {
      title += String(name);
      if (savedState != (setting.bitstate & (1 << index)))
        title += String("(bootme)");
      return title.c_str();
    }
};

class MenuItemLoco: public MenuItem {
  public:
    MenuItemLoco(): MenuItem(NULL, 0) {}

    void render(char *line, size_t size) {
      static const char fmt1[] = "Loco             %-4s";
      snprintf(line, size, fmt1, comms.getSelectedName());
    }

    void toggle() {
      comms.cycleSelected();
      comms.subsribe();
    }
};

auto mi0 = MenuItemLoco();
auto mi1 = MenuItemToggle("Lights", 0);
auto mi2 = MenuItemToggle("Slow", 1);
auto mi3 = MenuItemToggle("Pid", 2);
auto mi4 = MenuItemToggle("Shmak", 3);
auto mi5 = MenuItemToggleConfig("Big font", 1);
auto mi6 = MenuItemToggleLocal("Local", 0);
MenuItem *menuItem[] = {&mi0, &mi1, &mi2, &mi3, &mi4, &mi5, &mi6};

class MenuScreen : public BaseState {
  public:
    void setup() {
      for (int i = 0; i < sizeof(menuItem)/sizeof(menuItem[0]); i++)
        menuItem[i]->setup();
    }

    State handle(char key) {
      if (key == 'h')
        return STATE_HOME;

      static int selected = 0;
      if (key == 'd')
        if (selected < sizeof(menuItem)/sizeof(menuItem[0]) - 1)
          selected++;
        else
          selected = 0;
      else if (key == 'm')
        menuItem[selected]->toggle();

      int base = 0;
      if (selected >= MenuItem::LINES)
        base = selected - MenuItem::LINES + 1;
      ui.startScreen();
      for (int i = base; i < base + MenuItem::LINES; i++) {
        char line[ui.width + 1];
        ui.textColor(selected == i);
        menuItem[i]->render(line, sizeof(line));
        ui.display.println(line); 
      }
      ui.display.display();
      return STATE_NONE;
    }
};

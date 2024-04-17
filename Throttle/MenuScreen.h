#pragma once

extern UserInterface ui;

class MenuItem {
  protected:
    const char *name;
    const int index;
    int state;  
  public:
    MenuItem(const char *name, const int index): name(name), index(index), state(0) {}
    virtual void render(char *line, size_t size) = 0;
    virtual void toggle() = 0;
};

class MenuItemToggle: public MenuItem {
  public:
    MenuItemToggle(const char *name, const int index): MenuItem(name, index) {}

    void render(char *line, size_t size) {
      static const char fmt1[] = "%-18s%s";
      if (state)
        snprintf(line, size, fmt1, name, "ON");
      else
        snprintf(line, size, fmt1, name, "OFF");        
    }

    void toggle() {
      if (state)
        state = 0;
      else
        state = 1;
      Serial.println(state);
    }
};

auto mi1 = MenuItemToggle("Lights", 1);
auto mi2 = MenuItemToggle("Slow", 1);
auto mi3 = MenuItemToggle("Sound", 1);
auto mi4 = MenuItemToggle("Dude!", 1);
MenuItem *menuItem[] = {&mi1, &mi2, &mi3, &mi4};

class MenuScreen {
  public:
    void draw(char key) {
      static int base = 0;
      if (key == 'd')
        if (base < 3)
          base++;
        else
          base = 0;
      else if (key == 'm')
        menuItem[base]->toggle();

      ui.startScreen();
      for (int i=0; i<4; i++) {
        char line[ui.width + 1];
        ui.textColor(base == i);
        menuItem[i]->render(line, sizeof(line));
        ui.display.println(line); 
      }
      ui.display.display();
    }
};

/*
byte menuStatus[4];
const char *locoNames[3] = {"1204", " 948", " 002"};

void renderLoco(char* line, int index) {
  memset(line, ' ', WIDTH);
  memcpy(line, menuItem[index].name, strlen(menuItem[index].name));
  memcpy(line + WIDTH - 4, locoNames[menuStatus[index]], 4);
  line[WIDTH] = 0;
}
*/

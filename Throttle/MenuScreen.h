#pragma once

#include "States.h"
#include "Storage.h"
#include "UI.h"

extern UserInterface ui;
extern Storage storage;

class MenuItem
{
protected:
    const char *name;
    const int index;

public:
    static const int LINES = 4;
    MenuItem(const char *name, const int index) : name(name), index(index) {}
    virtual void setup() {};
    virtual void render(char *line, size_t size) = 0;
    virtual void toggle() = 0;
};

class MenuItemToggle : public MenuItem
{
public:
    MenuItemToggle(const char *name, const int index) : MenuItem(name, index) {}

    void render(char *line, size_t size)
    {
        static const char fmt1[] = "%-18s%s";
        if (pad.state.bitstate & (1 << index))
            snprintf(line, size, fmt1, name, "ON");
        else
            snprintf(line, size, fmt1, name, "OFF");
    }

    void toggle()
    {
        int state = (pad.state.bitstate & (1 << index));
        if (state)
            state = 0;
        else
            state = 1;
        pad.setFunction(index, state);
        pad.askHeartbeat();
    }
};

class MenuItemToggleConfig : public MenuItem
{
    int savedState;

public:
    MenuItemToggleConfig(const char *name)
        : MenuItem(name, 0)
    {
    }

    void render(char *line, size_t size)
    {
        static const char fmt1[] = "%-18s%s";
        if (settings.getCachedInt(name))
            snprintf(line, size, fmt1, name, "ON");
        else
            snprintf(line, size, fmt1, name, "OFF");
    }

    void toggle()
    {
        int c = settings.getCachedInt(name);
        settings.put(name, String(c ? 0 : 1));
    }
};

class MenuItemToggleLocal : public MenuItem
{
    int saved;
    const char* settingName = "locoaddr";
    const int node = 1;

public:
    MenuItemToggleLocal()
        : MenuItem("", 0)
    {
    }

    void setup()
    {
        saved = settings.getCachedInt(settingName);
    }

    void render(char *line, size_t size)
    {
        int c = settings.getCachedInt(settingName);
        if (c != saved)
            snprintf(line, size, "Local(bootme)     %s", c ? "OFF" : "ON");
        else
            snprintf(line, size, "Loco              %s", c ? "OFF" : "ON");
    }

    void toggle()
    {
        int c = settings.getCachedInt(settingName);
        settings.put(settingName, String(c ? 0 : node));
    }
};

class MenuItemLoco : public MenuItem
{
public:
    MenuItemLoco() : MenuItem(NULL, 0) {}

    void render(char *line, size_t size)
    {
        static const char fmt1[] = "Loco             %-4s";
        snprintf(line, size, fmt1, pad.getSelectedName());
    }

    void toggle()
    {
        pad.cycleSelected();
    }
};

auto mi0 = MenuItemLoco();
auto mi1 = MenuItemToggle("Lights", 0);
auto mi2 = MenuItemToggle("Bell", 1);
auto mi3 = MenuItemToggle("Slow", 28);
auto mi4 = MenuItemToggle("Pid", 29);
auto mi5 = MenuItemToggleConfig("BigFont");
auto mi6 = MenuItemToggleLocal();
MenuItem *menuItem[] = {&mi0, &mi1, &mi2, &mi3, &mi4, &mi5, &mi6};

class MenuScreen : public BaseState
{
public:
    void setup()
    {
        for (int i = 0; i < sizeof(menuItem) / sizeof(menuItem[0]); i++)
            menuItem[i]->setup();
    }

    State handle(char key)
    {
        if (key == 'h')
            return STATE_HOME;

        static int selected = 0;
        if (key == 'd')
            if (selected < sizeof(menuItem) / sizeof(menuItem[0]) - 1)
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

#pragma once
#include "RCCState.h"
#include "RCCPad.h"

struct Controls {
    uint16_t throttle;
    uint16_t direction;
    uint32_t timerBase;
    uint8_t pump;
};
extern struct Controls controls;

class Pad : public RCCPad 
{
public:

    void processGet(char cmd[])
    {
        if (strlen(cmd) < 1)
            return;
        char *key = cmd;
        String value = getValueLocal(key);
        Serial.println(String(key) + ":" + value);
        return;
    }

    void processSet(char* cmd)
    {
        if (strlen(cmd) < 2)
            return;
        char *separator = strchr(cmd, ':');
        if (separator == NULL)
            return;
        *separator = '\0';
        char *key = cmd;
        char *value = ++separator;
        setValueLocal(key, value);
    }

    virtual void onCommand(uint8_t code, char* value, uint8_t size)
    {
        value[size] = '\0';
        switch (code) {
        case 'G':
            processGet(value);
            break;
        case 'S':
            processSet(value);
            break;
        }
    }

    virtual void onSetValue(const char *key, const char *value)
    {
        if (strcmp(key, "pump") == 0)
            controls.pump = atoi(value);
    }
};

extern Pad pad;


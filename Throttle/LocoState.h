#pragma once
#include "RCCState.h"
#include "RCCKeypad.h"

// extern struct LocoState loco;

struct Controls {
    uint16_t throttle;
    uint16_t direction;
    uint16_t lost;
    uint32_t timerBase;
};
extern struct Controls controls;

struct Setting {
    union {
        uint16_t bitstate;
        struct {
            uint16_t local    : 1;
            uint16_t bigui    : 1;
            uint16_t reserved : 14;
        };
    };
};
extern struct Setting setting;

class TestKeypad : public RCCKeypad 
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
};

extern TestKeypad keypad;


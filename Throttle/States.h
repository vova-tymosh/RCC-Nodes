#pragma once

enum State { STATE_NONE = 0, STATE_HOME = 1, STATE_MENU = 2 };
const char ON_ENTER = '\n';

class BaseState
{
public:
    virtual State handle(char key)
    {
        return STATE_NONE;
    };
};

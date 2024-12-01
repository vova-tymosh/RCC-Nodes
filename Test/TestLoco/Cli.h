#include <stdint.h>

class CliBase
{
private:
    static const byte INPUT_LEN_MAX = 12;
    static const char END_MARKER = '\n';

    static const char CMD_DCC = 'D';
    static const char CMD_SPEED = 'S';
    static const char CMD_FUNCTION = 'F';
    static const char CMD_READ = 'R';

    bool processLine(char cmd[])
    {
        byte len = strlen(cmd);
        if (len == 0)
            return false;
        char first = cmd[0];
        if (first == CMD_DCC && len > 1)
            return processCommand(cmd+1, true);
        else
            return processCommand(cmd, false);
    }

    bool processCommand(char cmd[], bool is_dcc)
    {
        char first = cmd[0];
        if (strlen(cmd) > 1) {
            if (first == CMD_READ)
                return processExe(cmd+1, is_dcc);
            else if (first == CMD_SPEED)
                return processSpeed(cmd+1, is_dcc);
            else if (first == CMD_FUNCTION)
                return processFunction(cmd+1, is_dcc);
        }
        return false;
    }

    bool processExe(char cmd[], bool is_dcc) 
    {
        if (strlen(cmd) == 0)
            return false;
        onExe(cmd[0]);
        return true;
    }

    bool processSpeed(char cmd[], bool is_dcc) 
    {
        if (strlen(cmd) == 0)
            return false;
        char dir_text[2] = {cmd[0], '\0'};
        uint8_t dir = (uint8_t)atoi(dir_text);
        int speed = 0;
        if (strlen(cmd) > 1) {
            speed = atoi(cmd + 1);
            if (speed > 100)
                return false;
        }
        onSpeed(dir, (uint8_t)speed, is_dcc);
        return true;
    }

    bool processFunction(char cmd[], bool is_dcc) 
    {
        if (strlen(cmd) < 2)
            return false;
        uint8_t function = (uint8_t)atoi(cmd + 1);
        onFunction(function, cmd[0] == '1', is_dcc);
        return true;
    }


public:

    virtual void onExe(uint8_t code) {}

    virtual void onSpeed(uint8_t direction, uint8_t throttle, bool is_dcc) {}

    virtual void onFunction(uint8_t code, bool value, bool is_dcc) {}

    bool process() 
    {
        bool result;
        char line[INPUT_LEN_MAX];
        byte i = 0;
        while (Serial.available() > 0) {
            char rc = Serial.read();
            if (rc == END_MARKER)
                break;
            else if (i < INPUT_LEN_MAX) {
                line[i] = rc;
                i++;
            }
        }
        line[i] = '\0';
        if (i) {
            result = processLine(line);
            Serial.print(line);
            Serial.println(result ? " ok" : " error");
        }
        return result;
    }
};


/*
 * Railroad communication
 * 
 *  
 */
#pragma once
#include "Timer.h"
#include "Wireless.h"

#define MAX_PACKET 128
#define MAX_LOCO 5
#define NAME_SIZE 5

extern struct Loco loco;

struct __attribute__((packed)) Command {
  uint8_t type;
  uint8_t cmd;
  float value;
};
struct __attribute__((packed)) Auth {
  uint8_t cmd;
  uint8_t addr;
};

class ThrComms {
  private:
    static const char PACKET_THR_AUTH = 'q';
    static const char PACKET_THR_SUB  = 's';
    static const char PACKET_THR_NORM = 'p';
    static const char PACKET_LOCO_NORM = 'n';
    static const char FUNCTION_BASE = '@';
    Wireless *wireless;
    Command command;
    Timer timer;


  public:
    int lost;
    int total;

    struct AvailableLoco {
      uint8_t addr;
      char name[NAME_SIZE];
    };
    struct AvailableLoco availableLocos[MAX_LOCO];
    int selectedLoco;
    int maxAvailable;

    ThrComms(Wireless *wireless) : wireless(wireless), timer(100) {};

    void subsribe(char locoAddr) {
      Serial.println("Subscribe to " + String((int)locoAddr));
      command.type = PACKET_THR_SUB;
      command.cmd = locoAddr;
      command.value = 0;
    }

    int getLostRate() {
      int lostRate = 0;
      if (total) {
        lostRate = 100 * lost / total;
        if (lostRate > 100)
          lostRate = 100;
      }
      return lostRate;
    }

    void send(char cmd, float value) {
      command.type = PACKET_THR_NORM;
      command.cmd = cmd;
      command.value = value;
      Serial.println("Send "+ String(cmd) + "/" + String(value));
    }

    void sendFunction(char function, int value) {
      function = FUNCTION_BASE + function;
      send(function, (float)value);
    }

    void handleAuthorizeRequest(char *packet, uint16_t size) {
      packet[size] = 0;
      int i = 0;
      char *token = strtok(packet, " ");
      while(token && i < MAX_LOCO) {
        availableLocos[i].addr = atoi(token);
        token = strtok(NULL, " ");
        if (token) {
          strcpy(availableLocos[i].name, token);
          token = strtok(NULL, " ");
        }
        i++;
      }
      maxAvailable = i - 1;
    }

    void setup() {
      wireless->setup();
      command.type = PACKET_THR_AUTH;
      timer.restart();
    }

    bool loop() {
        bool update = false;
        if (wireless->available()) {
          char packet[MAX_PACKET];
          uint16_t res = wireless->read(packet, sizeof(packet));
          if (res > 1) {
            char cmd = packet[0];
            size_t size = res - 1;
            switch (cmd) {
            case PACKET_THR_AUTH:
              Serial.println("Request to authorize");
              handleAuthorizeRequest(&packet[1], size);
              command.type = PACKET_THR_NORM;
              subsribe(availableLocos[selectedLoco].addr);
              break;
            case PACKET_LOCO_NORM:
              size_t size = res - 1;
              memcpy(&loco, &packet[1], size);
              // Serial.println("Update "+ String(size) + "/"+String(loco.tick));
              update = true;
              break;
            }
          }
        }
        if (timer.hasFired()) {
          total++;
          if (!wireless->write(&command, sizeof(command)))
            lost++;
        }
        return update;
    }
};
 

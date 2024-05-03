/*
 * Railroad communication
 * 
 *  
 */
#pragma once
#include "Timer.h"
#include "Wireless.h"
#include "LocoState.h"


#define MAX_PACKET 128
#define MAX_LOCO 5
#define NAME_SIZE 5


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
    static const char PACKET_LOCO_AUTH = 'r';
    static const char PACKET_THR_AUTH = 'q';
    static const char PACKET_THR_SUB  = 's';
    static const char PACKET_THR_NORM = 'p';
    static const char PACKET_LOCO_NORM = 'n';
    static const char FUNCTION_BASE = '@';
    Wireless *wireless;
    Command command;
    Timer timer;
    int node;

    struct AvailableLoco {
      uint8_t addr;
      char name[NAME_SIZE];
    };

    struct RegisteredLoco {
      struct AvailableLoco locos[MAX_LOCO];
      int selected;
      int max;
    } registered;

    int lost;
    int total;

  public:
    ThrComms(Wireless *wireless) : wireless(wireless), timer(100) {};

    uint16_t getLostRate() {
      uint16_t lostRate = 0;
      if (total) {
        lostRate = 100 * lost / total;
        if (lostRate > 100)
          lostRate = 100;
      }
      return lostRate;
    }

    char *getSelectedName() {
      return registered.locos[registered.selected].name;
    }

    int getSelectedAddr() {
      return registered.locos[registered.selected].addr;
    }

    void cycleSelected() {
      if (registered.selected < registered.max)
        registered.selected++;
      else
        registered.selected = 0;
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

    void subsribe() {
      int addr = 1; //if no locos register try to subsribe to the 1st one
      if (registered.max > 0)
        addr = registered.locos[registered.selected].addr;
      command.type = PACKET_THR_SUB;
      command.cmd = addr;
      command.value = 0;
      Serial.println("Subscribe to " + String(addr));
    }

    void handleAuthorizeRequest(char *packet, uint16_t size) {
      Serial.println("Request to authorize");
      packet[size] = 0;
      int i = 0;
      char *token = strtok(packet, " ");
      while(token && i < MAX_LOCO) {
        registered.locos[i].addr = atoi(token);
        token = strtok(NULL, " ");
        if (token) {
          strcpy(registered.locos[i].name, token);
          token = strtok(NULL, " ");
        }
        i++;
      }
      registered.max = i - 1;
    }

    void askToAuthorize(int from) {
      command.type = PACKET_LOCO_AUTH;
      command.cmd = 0;
      command.value = 0;
      Serial.println("Local mode. Ask to auth");
    }

    bool isLocalMode() {
      return (node == 0);
    }

    bool isRegistered(int addr) {
      for (int i = 0; i < registered.max; i++) {
        if (addr == registered.locos[i].addr)
          return true;
      }
      return false;
    }

    bool handleNormal(char *payload, uint16_t size, int from) {
      bool mine = false;
      if (isLocalMode()) {
        if (from == getSelectedAddr())
          mine = true;
        else
          if (!isRegistered(from))
            askToAuthorize(from);
      } else {
        mine = true;
      }
      if (mine) {
        memcpy(&loco, payload, size);
        // Serial.println("Update "+ String(size) + "/"+String(loco.tick));
      }
      return mine;
    }

    void setup(int node) {
      this->node = node;
      wireless->setup(node);
      command.type = PACKET_THR_AUTH;
      timer.restart();
    }

    bool loop() {
        bool update = false;
        if (wireless->available()) {
          char packet[MAX_PACKET];
          int from;
          uint16_t res = wireless->read(packet, sizeof(packet), &from);
          if (res > 1) {
            char cmd = packet[0];
            char *payload = packet + 1;
            size_t size = res - 1;
            switch (cmd) {
            case PACKET_THR_AUTH:
              if (!isLocalMode()) {
                handleAuthorizeRequest(payload, size);
                subsribe();
              }
              break;
            case PACKET_LOCO_NORM:
              update = handleNormal(payload, size, from);
              break;
            }
          }
        }
        if (timer.hasFired()) {
          total++;
          if (!wireless->write(&command, sizeof(command), node))
            lost++;
        }
        return update;
    }
};
 

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
      Serial.println("Subscribe to " + String(addr));
      command.type = PACKET_THR_SUB;
      command.cmd = addr;
      command.value = 0;
    }

    void handleAuthorizeRequest(char *packet, uint16_t size) {
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

    void setup(int node) {
      wireless->setup(node);
      command.type = PACKET_THR_AUTH;
      timer.restart();
    }

    bool loop() {
        bool update = false;
        if (wireless->available()) {
          char packet[MAX_PACKET];
          int from;
          uint16_t res = wireless->readFrom(&from, packet, sizeof(packet));
          if (res > 1) {
            char cmd = packet[0];
            char *payload = packet + 1;
            size_t size = res - 1;
            switch (cmd) {
            case PACKET_THR_AUTH:
              Serial.println("Request to authorize");
              handleAuthorizeRequest(payload, size);
              subsribe();
              break;
            case PACKET_LOCO_NORM:
              if (from == 0) {//TODO
                memcpy(&loco, payload, size);
                // Serial.println("Update "+ String(size) + "/"+String(loco.tick));
                update = true;
              }
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
 

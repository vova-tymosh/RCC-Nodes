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
#define NAME_SIZE 4

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
    static const char PACKET_THR_AUTH = 's';
    static const char PACKET_THR_NORM = 'p';
    static const char PACKET_LOCO_NORM = 'n';
    static const char FUNCTION_BASE = '@';
    Wireless *wireless;
    Command command;
    Timer timer;

  public:
    int lost;
    int total;

    struct Loco {
      uint8_t addr;
      char name[NAME_SIZE];
    };
    struct Loco availableLocos[MAX_LOCO];

    ThrComms(Wireless *wireless) : wireless(wireless), timer(100) {};

    void authorize(byte locoAddr) {
      struct Auth cmd = {PACKET_THR_AUTH, locoAddr};
      Serial.println("Auth/Subscribe to " + String(locoAddr));
      wireless->write(&cmd, sizeof(cmd));
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
      command.cmd = cmd;
      command.value = value;
      // Serial.println("Send "+ String(cmd) + "/" + String(value));
    }

    void sendFunction(char function, int value) {
      function = FUNCTION_BASE + function;
      send(function, (float)value);
    }

    //TODO improve/test switches between locos
    void handleAuthorizeRequest(byte *packet, uint16_t size) {
      char message[32];
      memcpy(message, packet, size);
      message[size+1] = 0;
      Serial.println("handleAuthorizeRequest " + String(message));
      availableLocos[0].addr = 1;
      strcpy(availableLocos[0].name, "t001");
      /*
      byte *end = packet + size; 
      packet++; //skip 'r'
      int index = 0;
      Serial.println("handleAuthorizeRequest " + String(size));
      while (packet < end && index < MAX_LOCO) {
        loco = availableLocos[index++];
        loco.addr = *packet++;
        memcpy(loco.name, packet, NAME_SIZE);
        packet+= NAME_SIZE;
        S
        erial.println("handleAuthorizeRequest, step " + String(loco.addr) + "/" + String(loco.name));
      }
      */
    }

    void setup() {
      wireless->setup();
      command.type = PACKET_THR_NORM;
      timer.restart();
    }

    bool loop() {
        bool update = false;
        if (wireless->available()) {
          byte packet[MAX_PACKET];
          uint16_t res = wireless->read(packet, sizeof(packet));
          if (res > 1) {
            char cmd = packet[0];
            switch (cmd) {
            case PACKET_THR_AUTH:
              Serial.println("Request to authorize");
              handleAuthorizeRequest(packet , res);
              authorize(availableLocos[0].addr);
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
 

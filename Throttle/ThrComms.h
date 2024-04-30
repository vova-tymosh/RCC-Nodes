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
    static const int MAX_RETRY = 3;
    Wireless *wireless;
    Command queue;
    bool isQueueFull;
    int queueRetries;

    bool write(const void* payload, uint16_t size) {
      return wireless->write(payload, size);
    }
  public:
    int lost;
    int sent;
    int recv;

    struct Loco {
      uint8_t addr;
      char name[NAME_SIZE];
    };
    struct Loco availableLocos[MAX_LOCO];

    ThrComms(Wireless *wireless) : wireless(wireless) {};

    void authorize(byte locoAddr) {
      struct Auth cmd = {PACKET_THR_AUTH, locoAddr};
      Serial.println("Auth/Subscribe to " + String(locoAddr));
      write(&cmd, sizeof(cmd));
    }

    void send(char cmd, float value) {
      if (isQueueFull) {
        Serial.println("Queue is full");
      } else {
        queue.type = PACKET_THR_NORM;
        queue.cmd = cmd;
        queue.value = value;
        isQueueFull = true;
        queueRetries = MAX_RETRY;
        Serial.println("Enqueue " + String(cmd) + " " + String(value));
      }
    }

    void doSend() {
      if (isQueueFull) {
        if (write(&queue, sizeof(queue))) {
          isQueueFull = false;
          Serial.println("Sent " + String(queue.cmd) + " " + String(queue.value));
          sent++;
        } else {
          if (--queueRetries <= 0) {
            Serial.println("Queue timed out");
            isQueueFull = false;
            lost++;
          }
        }
      }
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
              recv++;
              // Serial.println("Update "+ String(size) + "/"+String(loco.tick));
              update = true;
              break;
            }
          }
        }
        doSend();
        return update;
    }
};
 

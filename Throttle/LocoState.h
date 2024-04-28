#pragma once

// const char *locoFields = "Time Bitstate Lost Throttle Speed Disatnce Battery Temp Psi";
struct __attribute__((packed)) Loco {
  uint16_t tick;
  union {
    uint16_t bitstate;
    struct {
      uint16_t lights    : 1;
      uint16_t slow      : 1;
      uint16_t pid       : 1;
      uint16_t reserved  : 12;
      uint16_t direction : 1;
    };
  };
  uint16_t lost;
  uint16_t throttle;
  int16_t  speed;
  uint16_t disatnce;
  uint16_t battery;
  int16_t  temperature;
  uint16_t psi;
};
extern struct Loco loco;

struct Controls {
  uint16_t throttle;
  int16_t direction;
};
extern struct Controls controls;


#pragma once

struct __attribute__((packed)) Loco {
  uint16_t tick;
  union {
    uint16_t bitstate;
    struct {
      uint16_t lights    : 1;
      uint16_t slow      : 1;
      uint16_t pid       : 1;
      uint16_t reserved  : 11;
      uint16_t direction : 2;
    };
  };
  uint16_t lost;
  uint16_t throttle;
  int16_t  speed;
  uint16_t disatnce;
  uint16_t battery;
  int16_t  temperature;
  uint16_t psi;
  uint16_t water;
};
extern struct Loco loco;

struct Controls {
  uint16_t throttle;
  uint16_t direction;
  uint16_t lost;
};
extern struct Controls controls;

struct Setting {
  union {
    uint8_t bitstate;
    struct {
      uint8_t local     : 1;
      uint8_t bigui     : 1;
      uint8_t reserved  : 6;
    };
  };
};
extern struct Setting setting;


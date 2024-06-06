#pragma once
#include "RCCState.h"

extern struct LocoState loco;

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


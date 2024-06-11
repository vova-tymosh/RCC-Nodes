#pragma once
#include "RCCState.h"

extern struct LocoState loco;

struct Controls {
  uint16_t throttle;
  uint16_t direction;
  uint16_t lost;
  uint32_t timerBase;
};
extern struct Controls controls;

struct Setting {
  union {
    uint16_t bitstate;
    struct {
      uint16_t local     : 1;
      uint16_t bigui     : 1;
      uint16_t reserved  : 14;
    };
  };
};
extern struct Setting setting;


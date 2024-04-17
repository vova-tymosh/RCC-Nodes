#pragma once

struct Loco {
  char *name;
  byte lost;
  byte battery;  //local
  int throttle; //out
  byte speed;
  int8_t direction; //out
  byte temperature;
  byte psi;
  int odo; 
  int light;
};

extern struct Loco loco;

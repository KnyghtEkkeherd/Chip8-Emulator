#ifndef Chip_H
#define Chip_H

#include "hardware_const.h"
#include "ram.h"
#include "display.h"
#include <cwctype>
#include <stack>
#include <sys/types.h>

class Chip {
  RAM ram;
  u_int16_t PC;
  u_int16_t I_reg;
  std::stack<u_int16_t> stack;
  u_int8_t delay_timer;
  u_int8_t sound_timer;
  u_int8_t V_reg[16];
  int state;
  public:
  Chip();
  void run();
};


#endif

#ifndef Chip_H
#define Chip_H

#include "hardware_const.h"
#include "ram.h"
#include "display.h"
#include <bitset>
#include <cstdlib>
#include <cwctype>
#include <stack>
#include <string>
#include <sys/types.h>
#include <fstream>

class Chip {
  RAM ram;
  Display display;
  u_int16_t PC;
  u_int16_t I_reg;
  std::stack<u_int16_t> stack;
  u_int8_t delay_timer;
  u_int8_t sound_timer;
  u_int8_t V_reg[16];
  u_int16_t instruction;

  // Compute loop
  void fetch();
  void decode();
  public:
  Chip();
  void run();
  void load_rom(std::string file_path);
};


#endif

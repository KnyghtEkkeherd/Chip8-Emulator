#ifndef Chip_H
#define Chip_H

#include "ram.hpp"
#include "display.hpp"
#include <fstream>
#include <stack>
#include <string>
#include <sys/types.h>
#include <iostream>
#include <fstream>

namespace ChipEmulator{

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
      const int** run();
      void load_rom(std::string file_path);
      void display_memory();
      int get_display_width() const;
      int get_display_height() const;
    };
}


#endif

#ifndef Chip_H
#define Chip_H

#include "ram.hpp"
#include "display.hpp"
#include "keyboard.hpp"
#include <fstream>
#include <stack>
#include <string>
#include <sys/types.h>
#include <iostream>
#include <fstream>

namespace ChipEmulator{

    class Chip {
      RAM ram;
      Keyboard keyboard;
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
      void draw(u_int16_t x_coord, u_int16_t y_coord, u_int16_t sprite_size);

      public:
      Chip();
      Display display;
      void run();
      void update_timers();
      void load_rom(const std::string file_path);
      void display_memory();
      int get_display_width() const;
      int get_display_height() const;
    };
}


#endif

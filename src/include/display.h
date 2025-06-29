#ifndef DISP_H
#define DISP_H

#include "hardware_const.h"
#include "ram.h"
#include <bitset>
#include <string>
#include <string>
#include <sys/types.h>
#include <vector>

class Display{
  private:
  u_int16_t font_start_idx;
  u_int16_t font[16][5];
  std::bitset<DISP_HEIGHT * DISP_WIDTH> window;
  std::string pixel;

  int draw_character(char character);
  int clear_line();
  int clear_screen();

  public:
  Display(RAM& ram);
  void update_window();
};

#endif

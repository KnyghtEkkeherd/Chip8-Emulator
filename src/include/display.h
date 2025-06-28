#ifndef DISP_H
#define DISP_H

#include "hardware_const.h"
#include "ram.h"
#include <string>
#include <string>
#include <sys/types.h>

class Display{
  private:
  u_int16_t font_start_idx;
  u_int16_t font[16][5];
  std::string pixel;
  int draw_character(char character);
  int clear_line();
  int clear_screen();

  public:
  Display(RAM& ram);
};

#endif

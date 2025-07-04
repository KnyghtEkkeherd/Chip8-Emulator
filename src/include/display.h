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

  public:
  Display(RAM& ram);
  void clear_screen();
  bool get_pixel(u_int16_t x_coord, u_int16_t y_coord);
  void set_pixel(u_int16_t x_coord, u_int16_t y_coord, bool value);
  void update_window();
};

#endif

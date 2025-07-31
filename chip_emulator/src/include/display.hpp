#ifndef DISP_H
#define DISP_H

#include "ram.hpp"
#include <string>
#include <string>
#include <sys/types.h>

class Display{
  private:
  int height;
  int width;
  u_int16_t font_start_idx;
  std::string pixel;
  int* window;

  public:
  Display(RAM& ram, int height, int width);
  ~Display();
  void clear_screen();
  bool get_pixel(u_int16_t x_coord, u_int16_t y_coord) const;
  int get_display_width() const;
  int get_display_height() const;
  u_int16_t get_font_idx() const;
  void flip_pixel(u_int16_t x_coord, u_int16_t y_coord);
  void set_pixel(u_int16_t x_coord, u_int16_t y_coord, bool value);
  const int* get_window() const;
  void print_window() const;
};

#endif

#ifndef DISP_H
#define DISP_H

#include "ram.hpp"
#include <string>
#include <string>
#include <sys/types.h>

struct DisplayWindow {
    private:
    int width;
    int height;
    int** pixelValues;


    public:
    DisplayWindow(int width, int height) : width(width), height(height) {
        pixelValues = new int*[height];
        for (int i = 0; i < height; ++i) {
            pixelValues[i] = new int[width];
        }
    }

    ~DisplayWindow() {
        for (int i = 0; i < height; ++i) {
            delete[] pixelValues[i];
        }
        delete[] pixelValues;
    }


};

class Display{
  private:
  int height;
  int width;
  u_int16_t font_start_idx;
  u_int16_t font[16][5];
  std::string pixel;
  int** window;

  public:
  Display(RAM& ram, int height, int width);
  ~Display();
  void clear_screen();
  bool get_pixel(u_int16_t x_coord, u_int16_t y_coord) const;
  int get_display_width() const;
  int get_display_height() const;
  void flip_pixel(u_int16_t x_coord, u_int16_t y_coord);
  void update_window();
  const int** get_window() const;
};

#endif

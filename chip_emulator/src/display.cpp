#include "include/display.hpp"
#include <iostream>
#include <ostream>
#include <string>
#include <sys/types.h>

Display::Display(RAM& ram, int height, int width): height(height), width(width),
                font_start_idx(0x050), pixel("■"){

    // load the font into the memory
    u_int16_t font[] = {0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80};

    for (u_int16_t i=0; i<sizeof(font)/sizeof(u_int16_t); ++i){
        ram.write_address(font_start_idx+i, font[i]);
    }

    window = new int[height*width];
}

Display::~Display(){
    delete[] window;
}

void Display::clear_screen(){
    for (int indx=0; indx<height*width; indx++){
            window[indx] = 0;
    }
}

bool Display::get_pixel(u_int16_t x_coord, u_int16_t y_coord) const{
    return window[x_coord + width*y_coord];
}

int Display::get_display_width() const{
    return width;
}

int Display::get_display_height() const{
    return height;
}

void Display::flip_pixel(u_int16_t x_coord, u_int16_t y_coord){
    window[x_coord + width*y_coord] = !window[x_coord + width*y_coord];
    std::cout << "Pixel flipped!" << std::endl;
}

void Display::set_pixel(u_int16_t x_coord, u_int16_t y_coord, bool value){
    window[x_coord + width*y_coord] = value;
}

const int* Display::get_window() const {
    return const_cast<const int*>(window);
}

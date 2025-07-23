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

    window = new int*[height];
    for (int i=0; i < height; ++i){
        window[i] = new int[width];
    }
}

Display::~Display(){
    for (int i=0; i<height; ++i){
        delete[] window[i];
    }
    delete[] window;
}

void Display::clear_screen(){
    for (int row=0; row<height; row++){
        for (int col=0; col<width; col++){
            window[row][col] = 0;
        }
    }
}

bool Display::get_pixel(u_int16_t x_coord, u_int16_t y_coord) const{
    return window[y_coord][x_coord];
}

int Display::get_display_width() const{
    return width;
}

int Display::get_display_height() const{
    return height;
}

void Display::flip_pixel(u_int16_t x_coord, u_int16_t y_coord){
    window[y_coord][x_coord] = !window[y_coord][x_coord];
}

void Display::update_window(){
    // for printing in the console without GUI
    for (int row=0; row<height; row++){
        for (int col=0; col<width; col++){
            std::cout << (window[row][col] ? pixel: ".");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

const int** Display::get_window() const {
    return const_cast<const int**>(window);
}

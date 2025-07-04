#include "include/display.h"
#include "include/hardware_const.h"
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/types.h>

Display::Display(RAM& ram): font_start_idx(0x050), pixel("■"){

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

    // reset the display window
    std::system("clear");
    window.reset();
    update_window();
}

void Display::clear_screen(){
    window.reset();
}

bool Display::get_pixel(u_int16_t x_coord, u_int16_t y_coord){
    return window[DISP_WIDTH*y_coord + x_coord];
}

void Display::flip_pixel(u_int16_t x_coord, u_int16_t y_coord){
    window[DISP_WIDTH*y_coord + x_coord] = !window[DISP_WIDTH*y_coord + x_coord];
}

void Display::update_window(){
    std::string window_str = window.to_string();

    for (int y=0; y<DISP_HEIGHT; y++){
        for (int x=0; x<DISP_WIDTH; x++){
            if (window_str[y*DISP_WIDTH + x] == '1'){
                std::cout << pixel;
            }else{
                std::cout << ' ';
            }
        }
        std::cout<<std::endl;
    }
}

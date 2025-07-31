#include "include/chip.hpp"
#include "include/display.hpp"
#include "include/hardware_const.hpp"
#include <iostream>
#include <sys/types.h>

using namespace ChipEmulator;

Chip::Chip(): PC(0x200), I_reg(0), delay_timer(0), sound_timer(0), instruction(0), display(ram, 64, 32){
    for (int i=0; i<16; ++i){
        V_reg[i] = 0;
    }
}

void Chip::fetch(){
    // each instruction is 2 bytes
    instruction = ram.read_address(PC) | (ram.read_address(PC + 1) << 8);
    PC += 2;
}

void Chip::decode(){

    u_int16_t X = (instruction & 0x0F00) >> 8;
    u_int16_t Y = (instruction & 0x00F0) >> 4;
    u_int16_t N = instruction & 0x000F;
    u_int16_t NN = instruction & 0x00FF;
    u_int16_t NNN = instruction & 0x0FFF;

    // display draw
    u_int16_t x_coord = V_reg[X] % display.get_display_width();
    u_int16_t y_coord = V_reg[Y] % display.get_display_height();

    std::cout << "Decoding: " << std::hex << instruction << std::endl;
    std::cout << "X: " << std::hex << X << std::endl;
    std::cout << "Y: " << std::hex << Y << std::endl;
    std::cout << "N: " << std::hex << N << std::endl;
    std::cout << "NN: " << std::hex << NN << std::endl;
    std::cout << "NNN: " << std::hex << NNN << std::endl;

    switch (instruction & 0xF000) {
        case 0x0000:
        // clear screen
        display.clear_screen();

        // for console printing
        //display.update_window();
        std::cout << "Clear screen" << std::endl;
        break;

        case 0x1000:
        // jump
        PC = NNN;
        std::cout << "Jump" << std::endl;
        break;

        case 0x6000:
        // set register VX
        V_reg[X] = NN;
        std::cout << "Setting V" << X << " to " << NN << std::endl;
        break;

        case 0x7000:
        // add value to register VX
        V_reg[X] += NN;
        std::cout << "Adding " << NN << " to V" << X << std::endl;
        break;

        case 0xA000:
        // set index register I
        I_reg = NNN;
        std::cout << "Setting Index Register to " << NNN << std::endl;
        break;

        case 0xD000:
        draw(x_coord, y_coord, N);
        // for console printing
        //display.update_window();
        break;

        default:
        std::cout << "Uknown Command!" << std::endl;
        break;
    }
    std::cout << "==========" << std::endl;
}

void Chip::run(){
    fetch();
    decode();
}

const int* Chip::get_window(){
    return display.get_window();
}

void Chip::load_rom(std::string file_path){
    std::ifstream file(file_path, std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    u_int8_t buffer;
    u_int16_t mem_address = 0;
    while (file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer))) {
        ram.write_address(PC+mem_address, buffer);
        std::bitset<8> buffer_print(buffer);
        /*
        std::cout << "Writing into ROM: " << std::hex << buffer_print.to_ulong() <<
            " at " << std::hex << PC+mem_address << std::endl;
        */
        mem_address++;
    }
    file.close();
    std::cout << "ROM file: " << file_path << " loaded successfuly!" << std::endl;
}

void Chip::display_memory(){
    std::cout << "Address\tValue\tAddress\tValue\tAddress\tValue\tAddress\tValue" << std::endl; // Header

    for (uint16_t address = 0x000; address < 0xfff; address += 4) {
        std::cout << std::hex << address << '\t'
                  << static_cast<int>(ram.read_address(address)) << '\t'
                  << std::hex << (address + 1) << '\t'
                  << static_cast<int>(ram.read_address(address + 1)) << '\t'
                  << std::hex << (address + 2) << '\t'
                  << static_cast<int>(ram.read_address(address + 2)) << '\t'
                  << std::hex << (address + 3) << '\t'
                  << static_cast<int>(ram.read_address(address + 3)) << std::endl;
    }
}

int Chip::get_display_width() const{
    return display.get_display_width();
}

int Chip::get_display_height() const{
    return display.get_display_height();
}

void Chip::draw(u_int16_t x_coord, u_int16_t y_coord, u_int16_t sprite_size){
    V_reg[0xF] = 0; // VF flag register
    u_int8_t pixel;
    for (u_int16_t yline = 0; yline < sprite_size; yline++){
        pixel = ram.read_address(I_reg + yline);
        for (u_int16_t xline = 0; xline < 8; xline++){
            if ((pixel & (0x80 >> xline)) != 0){
                if (display.get_pixel(x_coord + xline, y_coord + yline) == 1){
                    V_reg[0xF] = 1;
                    display.flip_pixel(x_coord + xline, y_coord + yline);
                }
            }
        }
    }
    std::cout << "Drawing" << std::endl;
}

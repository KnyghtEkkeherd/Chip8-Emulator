#include "include/chip.h"
#include "include/display.h"
#include "include/hardware_const.h"
#include <bitset>
#include <ios>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <iomanip>

Chip::Chip(): PC(0x200), I_reg(0), delay_timer(0), sound_timer(0), instruction(0), display(ram){
    for (int i=0; i<16; ++i){
        V_reg[i] = 0;
    }
}

void Chip::fetch(){
    // each instruction is 2 bytes
    instruction = (ram.read_address(PC) << 8);
    instruction = ram.read_address(PC+1) | instruction;
    PC += 2;
}

void Chip::decode(){

    u_int16_t X = (instruction & 0x0F00) >> 8;
    u_int16_t Y = (instruction & 0x00F0) >> 4;
    u_int16_t N = instruction & 0x000F;
    u_int16_t NN = instruction & 0x00FF;
    u_int16_t NNN = instruction & 0x0FFF;

    // display draw
    u_int16_t x_coord = V_reg[X] % DISP_WIDTH;
    u_int16_t y_coord = V_reg[Y] % DISP_HEIGHT;

    /*
    std::cout << "Decoding: " << std::hex << instruction << std::endl;
    std::cout << "X: " << std::hex << X << std::endl;
    std::cout << "Y: " << std::hex << Y << std::endl;
    std::cout << "N: " << std::hex << N << std::endl;
    std::cout << "NN: " << std::hex << NN << std::endl;
    std::cout << "NNN: " << std::hex << NNN << std::endl;
    */

    switch (instruction & 0xF000) {
        case 0x0000:
        // clear screen
        display.clear_screen();
        display.update_window();
        break;

        case 0x1000:
        // jump
        PC = NNN;
        break;

        case 0x6000:
        // set register VX
        V_reg[X] = NN;
        break;

        case 0x7000:
        // add value to register VX
        V_reg[X] += NN;
        break;

        case 0xA000:
        // set index register I
        I_reg = NNN;
        break;

        case 0xD000:
        V_reg[0xF] = 0; // VF flag register
        for (u_int16_t row=0; row < N; row++){
            std::bitset<8> sprite_data(ram.read_address(I_reg + row));
            for (u_int16_t col=0; col < 8; col++){
                if (display.get_pixel(x_coord, y_coord) == 1 && sprite_data[col] == 1){
                    V_reg[0xF] = 1;
                    display.flip_pixel(x_coord, y_coord);
                }
                else if( (display.get_pixel(x_coord, y_coord)) == 0 && sprite_data[col] == 1){
                    display.flip_pixel(x_coord, y_coord);
                }
                if (x_coord >= DISP_WIDTH-1){
                    break;
                }
                x_coord++;
            }
            y_coord++;
            if (y_coord >= DISP_HEIGHT-1){
                break;
            }
        }
        display.update_window();
        break;

        default:
        std::cout << "Uknown Command!" << std::endl;
        break;
    }
}

void Chip::run(){
    fetch();
    decode();
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

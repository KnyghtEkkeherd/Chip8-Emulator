#include "include/chip.hpp"
#include "include/display.hpp"
#include "include/hardware_const.hpp"
#include <iostream>
#include <random>
#include <sys/types.h>

using namespace ChipEmulator;

Chip::Chip(): PC(0x200), I_reg(0), delay_timer(0), sound_timer(0), instruction(0), display(ram, 32, 64){
    for (int i=0; i<16; ++i){
        V_reg[i] = 0;
    }
}

void Chip::fetch(){
    // each instruction is 2 bytes
    instruction = ram.read_address(PC) << 8;
    instruction |= ram.read_address(PC+1);
    PC += 2;
}

void Chip::decode(){

    u_int16_t X = (instruction & 0x0F00) >> 8;
    u_int16_t Y = (instruction & 0x00F0) >> 4;
    u_int16_t N = instruction & 0x000F;
    u_int16_t NN = instruction & 0x00FF;
    u_int16_t NNN = instruction & 0x0FFF;

    // random number generation setup
    std::random_device random_seed;
    std::mt19937 gen(random_seed());
    std::uniform_int_distribution<> distribution(0, 0xFFF);

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
            switch(NN){
                case 0xE0:
                // clear screen
                display.clear_screen();
                //std::cout << "Clear screen" << std::endl;
                break;

                case 0xEE:
                // subroutine return
                PC = stack.top();
                stack.pop();
                //std::cout << "Subroutine call return" << std::endl;
                break;
            }
        break;

        case 0x1000:
        // jump
        PC = NNN;
        //std::cout << "Jump" << std::endl;
        break;

        case 0x2000:
        // subroutine call
        stack.push(PC);
        PC = NNN;
        //std::cout << "Subroutine call" << std::endl;
        break;

        case 0x3000:
        // skip one instruction if VX is equal to NN
        if (V_reg[X] == NN){
            PC += 2;
        }
        break;

        case 0x4000:
        // skip one instruction if VX is not equal to NN
        if (V_reg[X] != NN){
            PC += 2;
        }
        break;

        case 0x5000:
        // skip one instruction if VX is equal to VY
        if (V_reg[X] == V_reg[Y]){
            PC += 2;
        }
        break;

        case 0x9000:
        // skip one instruction if VX is not equal to VY
        if (V_reg[X] != V_reg[Y]){
            PC += 2;
        }
        break;

        case 0x6000:
        // set register VX
        V_reg[X] = NN;
        //std::cout << "Setting V" << X << " to " << NN << std::endl;
        break;

        case 0x7000:
        // add value to register VX
        V_reg[X] += NN;
        //std::cout << "Adding " << NN << " to V" << X << std::endl;
        break;

        case 0x8000:
        // arithmetic and bitwise operations
            switch(N){
                case 0x0:
                V_reg[X] = V_reg[Y];
                break;

                case 0x1:
                V_reg[X] = V_reg[X] | V_reg[Y];
                break;

                case 0x2:
                V_reg[X] = V_reg[X] & V_reg[Y];
                break;

                case 0x3:
                V_reg[X] = V_reg[X] ^ V_reg[Y];
                break;

                case 0x4:
                V_reg[X] = V_reg[X] + V_reg[Y];
                break;

                case 0x5:
                V_reg[X] = V_reg[X] - V_reg[Y];
                break;

                case 0x6:
                V_reg[X] = V_reg[X] >> 1;
                break;

                case 0x7:
                V_reg[X] = V_reg[Y] - V_reg[X];
                break;

                case 0xE:
                V_reg[X] = V_reg[X] << 1;
                break;
            }
        break;

        case 0xA000:
        // set index register I
        I_reg = NNN;
        //std::cout << "Setting Index Register to " << NNN << std::endl;
        break;

        case 0xB000:
        PC = NNN + V_reg[0x0];
        break;

        case 0xC000:
        V_reg[X] = distribution(gen) & NN;
        break;

        case 0xD000:
        draw(x_coord, y_coord, N);
        break;

        case 0xE000:
            switch(NN){
                case 0x9E:
                // skip an instruction if corresponding key is pressed
                break;
                case 0xA1:
                // skip an instruction if the corresponding key is not pressed
                break;
            }
        break;

        case 0xF000:
            switch(NN){
                case 0x07:
                V_reg[X] = delay_timer;
                break;

                case 0x15:
                delay_timer = V_reg[X];
                break;

                case 0x18:
                sound_timer = V_reg[X];
                break;

                case 0x1E:
                I_reg = I_reg + V_reg[X];
                break;

                case 0x0A:
                // wait for key input TODO
                PC -= 2;
                break;

                case 0x29:
                I_reg = display.get_font_start_idx() + V_reg[X];
                break;

                // TODO: 0x33
                case 0x55:
                    for (int i = 0; i <= X; ++i) {
                        ram.write_address(I_reg + i, V_reg[i]);
                    }
                    break;
                case 0x65:
                    for (int i = 0; i <= X; ++i) {
                        V_reg[i] = ram.read_address(I_reg + i);
                    }
                    break;
                default:
                std::cout << "Command not implemented!" << std::hex << instruction << std::endl;
            }
        break;

        default:
        std::cout << "Uknown opcode: " << std::hex << instruction << std::endl;
        break;
    }
}

void Chip::run(){
    fetch();
    decode();
    update_timers();
}

void Chip::update_timers(){
    if (delay_timer > 0)
        delay_timer--;
    if (sound_timer > 0){
        if (sound_timer == 1)
            std::cout << "BEEP" << std::endl;
        sound_timer--;
    }
}

void Chip::load_rom(const std::string file_path){
    std::ifstream file(file_path, std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    u_int8_t buffer[2];
    u_int16_t mem_address = 0;
    while (file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer))) {
        ram.write_address(PC+mem_address, buffer[1]);
        ram.write_address(PC+mem_address+1, buffer[0]);
        mem_address += 2;
        /*
        std::bitset<8> buffer_print(buffer[1]);
        std::cout << "Writing into ROM: " << std::hex << buffer_print.to_ulong() <<
            " at " << std::hex << PC+mem_address << std::endl;

        buffer_print = buffer[0];
        std::cout << "Writing into ROM: " << std::hex << buffer_print.to_ulong() <<
            " at " << std::hex << PC+mem_address+1 << std::endl;
        */
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
    V_reg[0xF] = 0;
    u_int16_t pixel;
    for (int yline = 0; yline < sprite_size; yline++){
        pixel = ram.read_address(I_reg + yline);
        for (int xline = 0; xline < 8; xline++){
            if( (pixel & (0x80 >> xline)) != 0){
                if (display.get_pixel(x_coord + xline, y_coord + yline) == 1){
                    V_reg[0xF] = 1;
                }
                display.flip_pixel(x_coord + xline, y_coord + yline);
            }
        }
    }
    //display.print_window();
}

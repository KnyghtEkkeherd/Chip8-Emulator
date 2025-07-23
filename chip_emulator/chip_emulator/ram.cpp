#include "include/ram.h"
#include "include/hardware_const.h"
#include <iostream>
#include <ostream>
#include <sys/types.h>

RAM::RAM(){
  for (int i=0; i < RAM_SIZE; ++i){
      memory[i] = 0;
  }
}

u_int8_t RAM::read_address(u_int16_t address){
    return memory[address];
}

void RAM::write_address(u_int16_t address, u_int8_t data){
    memory[address] = data;
}

void RAM::print_ram(){
    std::cout << "Address" << '\t' << "Data" << std::endl;
    for (u_int16_t i=0; i<RAM_SIZE; ++i){
        std::cout << std::hex << memory[i] << std::endl;
    }
}

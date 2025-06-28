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

bool RAM::verify_address(u_int16_t address){
    return (address < RAM_SIZE && address >= 0);
}

u_int16_t RAM::read_address(u_int16_t address){
    if (verify_address(address)){
        return memory[address];
    }

    std::cerr << "Address outside bounds" << std::endl;
    return 0;
}

int RAM::write_address(u_int16_t address, u_int16_t data){
    if (verify_address(address)){
        memory[address] = data;
        return 0;
    }

    std::cerr << "Address outside bounds" << std::endl;
    return -1;
}

void RAM::print_ram(){
    std::cout << "Address" << '\t' << "Data" << std::endl;
    for (u_int16_t i=0; i<RAM_SIZE; ++i){
        std::cout << i << std::hex << '\t' << memory[i] << std::endl;
    }
}

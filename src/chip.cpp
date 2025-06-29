#include "include/chip.h"
#include <iostream>

Chip::Chip(): PC(0), I_reg(0), delay_timer(0xff), sound_timer(0), state(0){
    for (int i=0; i<16; ++i){
        V_reg[i] = 0;
    }
}

void Chip::run(){
    // state:
    // 0 - fetch
    // 1 - decode
    // 2 - execute
    switch (state){
        case 0:
        std::cout << "Fetch " << state << std::endl;
        state++;
        break;
        case 1:
        std::cout << "Decode " << state << std::endl;
        state++;
        break;
        case 2:
        std::cout << "Execute " << state << std::endl;
        state = 0;
        break;
        default:
        std::cerr << "Error state! " << state << std::endl;
        break;
    }
}

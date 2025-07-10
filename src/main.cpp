#include "include/chip.h"
#include <chrono>
#include <iostream>
#include <sys/types.h>
#include <thread>

int main(){
    Chip chip;
    chip.load_rom("src/roms/2-ibm-logo.ch8");
    int cycles = 0;
    while (cycles != 20){
        chip.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        cycles++;
    }
    chip.display_memory();
    return 0;
}

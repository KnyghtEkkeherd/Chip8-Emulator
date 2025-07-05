#include "include/chip.h"
#include <chrono>
#include <sys/types.h>
#include <thread>

int main(){
    Chip chip;
    chip.load_rom("src/roms/IBMLogo.ch8");
    while (true){
        chip.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    return 0;
}

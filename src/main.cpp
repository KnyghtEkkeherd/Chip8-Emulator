#include "include/chip.h"
#include <chrono>
#include <thread>

int main(){
    Chip chip;
    while (true){
        chip.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    return 0;
}

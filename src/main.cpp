#include "include/ram.h"
#include "include/display.h"
#include <iostream>
#include <ostream>

int main(){
    RAM ram;
    Display disp(ram);
    ram.print_ram();
    return 0;
}

#ifndef RAM_H
#define RAM_H

#include <sys/types.h>
#include "hardware_const.h"

class RAM {
    private:
    u_int16_t memory[RAM_SIZE];
    bool verify_address(u_int16_t address);

    public:
    RAM();
    u_int16_t read_address(u_int16_t address);
    int write_address(u_int16_t address, u_int16_t data);
    void print_ram();

};
#endif

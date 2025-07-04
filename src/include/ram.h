#ifndef RAM_H
#define RAM_H

#include <bitset>
#include <cstdint>
#include <sys/types.h>
#include "hardware_const.h"

class RAM {
    private:
    u_int8_t memory[RAM_SIZE];
    bool verify_address(u_int16_t address);

    public:
    RAM();
    uint8_t read_address(u_int16_t address);
    int write_address(u_int16_t address, uint8_t data);
    void print_ram();

};
#endif

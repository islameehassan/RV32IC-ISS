#ifndef RV32IC_ISS_MEMORY_H
#define RV32IC_ISS_MEMORY_H

#include <iostream>
#include <cstdint>

// Memory is byte addressable
class Memory{

    // text section array assumed to start at 0x00000000 and has size = 64Kbytes
    // data section array assumed to start at 0x00010000 and has size = 16Kbytes
    uint8_t memory_array [81920];

public:
    // initializes memory with 0x0
    Memory();
    // read byte in memory starting in location
    uint8_t read_byte (int location);

    // read half word in memory starting from location
    uint16_t read_half_word (int location);

    // read word in memory starting from location
    uint32_t read_word (int location);

    // store byte in memory starting in location
    int store_byte (int location,uint8_t byte);

    // store half word in memory starting from location
    int store_half_word (int location,uint16_t half_word);

    // store word in memory starting from location
    int store_word (int location,uint32_t word);

    // load .text section into memory where it should have all instructions
    int load_text_section(std::string machine_code_fname);
};

#endif //RV32IC_ISS_MEMORY_H
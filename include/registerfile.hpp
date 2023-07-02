#ifndef RV32IC_ISS_REGISTERFILE_H
#define RV32IC_ISS_REGISTERFILE_H

#include <cstdint>
#include <iostream>

using std::string;
/*
* Handles read and write to registers in the simulated CPU
*/

// mnemonic names for the 32 registers
static const string regnames[] = {
    "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

class RegisterFile{
    // Register File
    int32_t regfile[32];
    const int& firstElement = regfile[0];

public:
    // initializes x0 with 0, and the rest with 0x1 (dummy value)
    RegisterFile();

    // set register "r" with value "value"
    int set(uint32_t r, int32_t value);

    // get the value stored in register "r"
    int32_t get(uint32_t r);

    // get the mnemonic name for register "r"
    string getMnemonic(uint32_t r);
};

#endif //RV32IC_ISS_REGISTERFILE_H
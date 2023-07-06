#include "../include/registerfile.hpp"
#include <cstring>

RegisterFile::RegisterFile(){
    memset(regfile, 0, sizeof(regfile));
}

int RegisterFile::set(uint32_t r, int32_t value){
    // out of bound
    if(r == 0)
        return 0;
    else if(r > 31)
        return -1;

    this->regfile[r] = value;
    return 1;   // successful
}

int32_t RegisterFile::get(uint32_t r){
    // out of bound
    if(r > 31)
        return -1;

    return regfile[r];
}

string RegisterFile::getMnemonic(uint32_t r){
    // out of bound
    if(r > 31)
        return "invalid";

    return regnames[r];
}
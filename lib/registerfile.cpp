#include "registerfile.hpp"

RegisterFile::RegisterFile(){
    // set x0 to zero
    regfile[0] = 0x0;

    // set the rest to 1
    for (int i = 1; i < 32; i++){
        regfile[i] = 0x1;
    }
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
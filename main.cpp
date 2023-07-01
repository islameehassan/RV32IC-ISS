#include <iostream>
#include "disassembler.h"
using std::cout;
using std::cin;
#include <fstream>

enum reg{
    Zero,
    sp,
    gp,
    tp,
    s0
};

int main(){
    reg r = reg::Zero;
    // << r << '\n';
    // uint8_t d1=0x01;
    // uint8_t d2=0x02;
    uint32_t  instruction = 0xff5ff06f;
    int32_t imm_i = (instruction & 0xFFFF00000) >> 20;
    if (instruction & 0x80000000) // sign extend
        imm_i |= 0xFFFFF000;
    int32_t imm_s = (int32_t)(instruction & 0xFFF00000) >> 20;
    imm_s |= (int32_t)(instruction & 0x00000F80) >> 7;
    imm_s = (instruction & 0xFFFFF000);
    int32_t imm_b = (int32_t)(instruction & 0x80000000) >> 19;
    imm_b |= (int32_t)(instruction & 0x00000080) << 4;
    imm_b |= (int32_t)(instruction & 0x7E000000) >> 20;
    imm_b |= (int32_t)(instruction & 0x00000F00) >> 7;
    int32_t imm_j = (int32_t)(instruction & 0x80000000) >> 11; // move bit 31 to 20
    imm_j |= (int32_t)(instruction & 0x7FE00000) >> 20;        // move bit 30 : 21 to 10 : 1
    imm_j |= (int32_t)(instruction & 0x00100000) >> 9;         // move bit 20 : 11
    imm_j |= (int32_t)(instruction & 0x000FF000);               // extract bits 19 : 12
    cout<<"xd";
    /*Memory mem;
    mem.store_half_word(81917,0x0FFD);
    std::ifstream inputFile("C:\\RV32IC-ISS\\test.txt", std::ios::binary);
    char temp;
    int counter = 0;
    std::ifstream infile("C:\\RV32IC-ISS\\test.txt", std::ios::in | std::ios::binary);
    //checks if file exists or can be opened
    if (!infile)
    {
        std::cout << "Can't open file " << "fname" << " for reading" << std::endl;
    }
    //reads in bit by bit
    while (infile.get(temp))
    {
        cout<<temp<<"\n";
    }
    /*
    std::ifstream is ("C:\\RV32IC-ISS\\test.txt", std::ifstream::binary);
    if (is) {
        // get length of file:
        is.seekg (0, is.end);
        int length = is.tellg();
        is.seekg (0, is.beg);

        char * buffer = new char [length];

        std::cout << "Reading " << length << " characters... ";
        // read data as a block:
        is.read (buffer,length);

        if (is)
            std::cout << "all characters read successfully.";
        else
            std::cout << "error: only " << is.gcount() << " could be read";
        is.close();

        // ...buffer contains the entire file...

        delete[] buffer;
    }
    */

}
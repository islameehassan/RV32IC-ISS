#include <iostream>
#include <iomanip>
#include "disassembler.h"
using std::cout;
using std::cin;

enum reg{
    Zero,
    sp,
    gp,
    tp,
    s0
};

using namespace std;

void printPrefix(unsigned int instA, unsigned int instW){
    cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW<<"\n";
}

int main(int argc, char *argv[]){
    disassembler dis("lol");
}
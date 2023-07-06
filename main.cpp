<<<<<<< HEAD
#include "disassembler.h"

int main(int argc, char *argv[]){

    disassembler dis("lol");
=======
#include <iostream>
#include <fstream>
#include <cassert>
#include "disassembler.hpp"


using std::cout;
using std::cin;

int main(int argc, char* argv[]){
    if(argc == 1){
        cout << "Usage: ./disassembler <machine_code_file>" << std::endl;
        return 1;
    }
    else{
        cout << "Number of arguments: " << argc << std::endl;
    }
    std::string data_section_fname = "";
    std::string machine_code_fname = argv[1];
    if(argc == 3){
        std::string data_section_fname = argv[2];
    }
    disassembler diss(machine_code_fname, data_section_fname);    
    // Testing compressed instructions
>>>>>>> 6716792 (finalized compressed extension)

    // uint16_t compressed_instructions[] = {
    //     0b0100010010011101,
    //     0b1000010011111010,
    //     0b1000011001100001,
    //     0b1001110110000010,
    //     0b0001001100001110,
    //     0b0000000000000001,
    //     0b1001110010001010,
    //     0b1000110011101101,
    //     0b1000110010101101,
    //     0b1000110011001101,
    //     0b1000110010001101,
    //     0b0100010011000010,
    //     0b1100100000101010,
    //     0b0100100110000100,
    //     0b1100100110001000,
    //     0b1010100000000001,
    //     0b0010100000000001,
    //     0b1000010110000010,
    //     0b1001010110000010,
    //     0b1100100110000001,
    //     0b1110100110000001,
    //     0b0100010010011101,
    //     0b0110010010000001,
    //     0b0000010011000101,
    //     0b0110000101000001,
    //     0b0000100000000100,
    //     0b0000010011000110,
    //     0b1000000011000101,
    //     0b1000011001100001,
    //     0b1000100011000101,
    // };

    // uint32_t uncompressed_instructions[] = {
    //     0b00000000011100000000010010010011,
    //     0b00000001111000000000010010110011,
    //     0b01000001100001100101011000010011,
    //     0b00000000000011011000000011100111,
    //     0b00000000000011011000000001100111,
    //     0b00000000001100110001001100010011,
    //     0b00000000000000000000000000010011,
    //     0b00000000101100000001000000010011,
    //     0b00000000101101001111010010110011,
    //     0b00000000101101001100010010110011,
    //     0b00000000101101001110010010110011,
    //     0b01000000101101001000010010110011,
    //     0b00000001000100010010010010000011,
    //     0b00000000101000010010100000100011,
    //     0b00000001000001011010010010000011,
    //     0b00000000101001011010100000100011,
    //     0b00000001000000000000000001101111,
    //     0b00000001000000000000000011101111,
    //     0b00000000000001011000000001100111,
    //     0b00000000000001011000000011100111,
    //     0b00000000000001011000100001100011,
    //     0b00000000000001011001100001100011,
    //     0b00000000011100000000010010010011,
    //     0b00000000000000000000010010110111,
    //     0b00000001000101001000010010010011,
    //     0b00000001000000010000000100010011,
    //     0b00000001000000010000010010010011,
    //     0b00000001000101001001010010010011,
    //     0b00000001000101001101010010010011,
    //     0b01000001100001100101011000010011,
    //     0b00000001000101001111010010010011,
    // };

    // int n = sizeof(compressed_instructions) / sizeof(compressed_instructions[0]);
    // disassembler diss;

    // uint32_t decoded_instruction = diss.decode_compressed_intstruction(0b0100010011000010);    
    // uint32_t true_instruction =  0b00000001000100010010010010000011;

    // uint32_t decoded_rs1 = diss.get_rd(decoded_instruction);
    // uint32_t true_rs1 = diss.get_rd(true_instruction);



    // cout << std::hex << decoded_instruction << std::endl;
    // cout << std::hex << true_instruction << std::endl;

    // cout << std::hex << decoded_rs1 << std::endl;
    // cout << std::hex << true_rs1 << std::endl;

    // diss.print_instruction(true_instruction);
    // diss.print_instruction(decoded_instruction);
}
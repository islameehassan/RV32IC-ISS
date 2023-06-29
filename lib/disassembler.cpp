#include "disassembler.h"

disassembler::disassembler(const std::string& machine_code_fname) {
    mem.load_text_section(machine_code_fname);
}

void disassembler::run() {
    // Set PC to the beginning of the text section
    // Load half a word per iteration to support compressed instructions
    for(PC = 0x00000000 ; PC < 0x00010000 ; PC+=2){
        uint16_t  half_word = mem.read_half_word(PC);
        if((half_word & 0x03) != 0x03){
            // it is a compressed instruction
        }else if((half_word & 0x03) == 0x03 && (half_word & 0x1C) != 0x1C) {

        }else
            continue;


    }
}

uint32_t disassembler::decode_compressed_intstruction(uint16_t cinstruction) {
    return 0;
}

void disassembler::execute(uint32_t instruction) {

}

uint32_t disassembler::get_opcode(uint32_t instruction) {
    // Return 0 - 6 bits
    return (instruction & 0x0000007F);
}

uint32_t disassembler::get_rd(uint32_t instruction) {
    // Return 7 - 11 bits
    return (instruction & 0x00000F80) >> 7;
}

uint32_t disassembler::get_func3(uint32_t instruction) {
    // Return 12 - 14 bits
    return (instruction & 0x00007000) >> 12;
}
uint32_t disassembler::get_rs1(uint32_t instruction) {
    // Return 15 - 19 bits
    return (instruction & 0x000F8000) >> 15;
}

uint32_t disassembler::get_rs2(uint32_t instruction) {
    // Return 20 - 24 bits
    return (instruction & 0x01F00000) >> 20;
}

uint32_t disassembler::get_func7(uint32_t instruction) {
    // Return 25 - 31 bits
    return (instruction & 0xFE000000) >> 25;
}

int32_t disassembler::get_imm_itype(uint32_t instruction) {
    // Return 20 - 31 bits
    // Typecasting to int32_2 then shifting performs sign extension for us automatically
    return (int32_t)(instruction & 0xFFF00000) >> 20;
}

int32_t disassembler::get_imm_utype(uint32_t instruction) {
    // Return 12 - 31 bits
    // In U-Type, the immediate is shifted to the left 12 bits
    return (int32_t)(instruction & 0xFFFFF000);
}

int32_t disassembler::get_imm_stype(uint32_t instruction) {
    // Return 25 - 31 bits
    // Typecasting to int32_2 then shifting performs sign extension for us automatically
    int32_t imm_s = (int32_t)(instruction & 0xFE000000) >> 20;
    imm_s |= (int32_t)(instruction & 0x00000F80) >> 7;
    return imm_s;
}

int32_t disassembler::get_imm_btype(uint32_t instruction) {
    // Typecasting to int32_2 then shifting performs sign extension for us automatically
    int32_t imm_b = (int32_t)(instruction & 0x80000000) >> 19; // move bit 31 to 12
    imm_b |= (int32_t)(instruction & 0x00000080) << 4;         // move bit 7 to 11
    imm_b |= (int32_t)(instruction & 0x7E000000) >> 20;        // move bits 30 : 25 to 10 : 5
    imm_b |= (int32_t)(instruction & 0x00000F00) >> 7;         // move bits 11 : 8 to 4 : 1
    return imm_b;
}

int32_t disassembler::get_imm_jtype(uint32_t instruction) {
    // Typecasting to int32_2 then shifting performs sign extension for us automatically
    int32_t imm_j = (int32_t)(instruction & 0x80000000) >> 11; // move bit 31 to 20
    imm_j |= (int32_t)(instruction & 0x7FE00000) >> 20;        // move bit 30 : 21 to 10 : 1
    imm_j |= (int32_t)(instruction & 0x00100000) >> 9;         // move bit 20 : 11
    imm_j |= (int32_t)(instruction & 0x000FF000);              // extract bits 19 : 12
    return imm_j;
}



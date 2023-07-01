#ifndef RV32IC_ISS_DISASSEMBLER_H
#define RV32IC_ISS_DISASSEMBLER_H
#include "registerfile.hpp"
#include "memory.hpp"
#include <fstream>

class disassembler {
    Memory mem;
    RegisterFile register_file;

    int PC;

    uint32_t get_opcode(uint32_t instruction);
    uint32_t get_rd(uint32_t instruction);
    uint32_t get_rs1(uint32_t instruction);
    uint32_t get_rs2(uint32_t instruction);
    uint32_t get_func7(uint32_t instruction);
    uint32_t get_func3(uint32_t instruction);
    uint32_t decode_compressed_intstruction(uint16_t cinstruction);

    int32_t  get_imm_itype(uint32_t instruction);
    int32_t  get_imm_stype(uint32_t instruction);
    int32_t  get_imm_btype(uint32_t instruction);
    int32_t  get_imm_utype(uint32_t instruction);
    int32_t  get_imm_jtype(uint32_t instruction);

    void execute(uint32_t instruction);

public:
    //
    disassembler (const std::string& machine_code_fname);
    void run();


};

#endif //RV32IC_ISS_DISASSEMBLER_H

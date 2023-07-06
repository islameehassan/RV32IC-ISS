#ifndef RV32IC_ISS_DISASSEMBLER_H
#define RV32IC_ISS_DISASSEMBLER_H
#include "registerfile.hpp"
#include "memory.hpp"
#include <fstream>

// opcodes for the base integer instructions
constexpr uint32_t R_opcode = 0b0110011;
constexpr uint32_t I_arth_opcode = 0b0010011;   // opcode fir addi, xori, ..
constexpr u_int32_t I_load_opcode = 0b000011;
constexpr u_int32_t I_jalr_opcode = 0b1100111;
constexpr u_int32_t I_syscall_opcode = 0b1110011;   // ecall and ebreak
constexpr u_int32_t S_opcode = 0b0100011;
constexpr u_int32_t B_opcode = 0b1100011;
constexpr u_int32_t Jal_opcode = 0b1101111;
constexpr u_int32_t U_lui_opcode = 0b0110111;
constexpr u_int32_t U_auipc_opcode = 0b0010111;

class disassembler {
public:
    Memory mem;
    RegisterFile register_file;

    int32_t PC;
    string instruction_name;

    bool is_cur_inst_compressed;
    // getters for base integer instruction fields
    uint32_t get_opcode(uint32_t instruction);
    uint32_t get_rd(uint32_t instruction);
    uint32_t get_rs1(uint32_t instruction);
    uint32_t get_rs2(uint32_t instruction);
    uint32_t get_func7(uint32_t instruction);
    uint32_t get_func3(uint32_t instruction);


    // getters for compressed instruction fileds
    u_int32_t get_C_opcode(uint16_t cinstruction);
    u_int32_t get_C_func3(uint16_t cinstruction);

    // decoders for compressed instructions
    u_int32_t decode_CI_I (uint16_t cinstruction, uint32_t func3);
    u_int32_t decode_CR (uint16_t cinstruction, uint32_t func3);
    u_int32_t decode_CSS (uint16_t cinstruction, uint32_t func3);
    u_int32_t decode_CIW (uint16_t cinstruction, uint32_t func3);
    u_int32_t decode_CL (uint16_t cinstruction, uint32_t func3);
    u_int32_t decode_CS (uint16_t cinstruction, uint32_t func3);
    u_int32_t decode_CB_B (uint16_t cinstruction, uint32_t func3);
    u_int32_t decode_CB_I (uint16_t cinstruction, uint32_t func3);
    u_int32_t decode_CJ (uint16_t cinstruction, uint32_t rd);
    uint32_t decode_CA (uint16_t cinstruction, uint32_t func3);
    uint32_t decode_CR_Jar (uint16_t cinstruction, uint32_t rd);
    uint32_t decode_CR_ADD(uint16_t cinstruction);
    uint32_t decode_CI_LWSP(uint16_t cinstruction);
    uint32_t decode_LUI(uint16_t cinstruction);
    uint32_t decode_CI_LUI(uint16_t cinstruction);
    
    // getters for immediate values
    int32_t  get_imm_itype(uint32_t instruction);
    int32_t  get_imm_stype(uint32_t instruction);
    int32_t  get_imm_btype(uint32_t instruction);
    int32_t  get_imm_utype(uint32_t instruction);
    int32_t  get_imm_jtype(uint32_t instruction);


    // execute and print functions for base integer instructions
    void execute_rtype(uint32_t instruction);
    void execute_stype(uint32_t instruction);
    void execute_btype(uint32_t instruction);
    void execute_jtype(uint32_t instruction);
    void execute_ecall();
    void execute_utype_lui (uint32_t instruction);
    void execute_utype_aui (uint32_t instruction);
    void execute_itype_arit(uint32_t instruction);
    void execute_itype_load(uint32_t instruction);
    void execute_itype_jalr(uint32_t instruction);
    void print_rtype(uint32_t instruction);
    void print_stype(uint32_t instruction);
    void print_btype(uint32_t instruction);
    void print_jtype(uint32_t instruction);
    void print_utype_lui (uint32_t instruction);
    void print_utype_aui (uint32_t instruction);
    void print_itype_arit(uint32_t instruction);
    void print_itype_load(uint32_t instruction);
    void print_itype_jalr(uint32_t instruction);
    void print_ecall();

    disassembler();
    explicit disassembler (const std::string& machine_code_fname, const std::string& data_section_fname);

    uint32_t decode_instruction(uint32_t instruction);

    void run();
    void print_instruction(uint32_t instruction);
    void print_string(int32_t address);

    uint32_t decode_compressed_intstruction(uint16_t cinstruction);

};

#endif //RV32IC_ISS_DISASSEMBLER_H
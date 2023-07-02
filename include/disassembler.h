#ifndef RV32IC_ISS_DISASSEMBLER_H
#define RV32IC_ISS_DISASSEMBLER_H
#include "registerfile.hpp"
#include "memory.hpp"


constexpr uint32_t R_opcode = 0b0110011;
constexpr uint32_t I_arth_opcode = 0b0010011;   // opcode fir addi, xori, ..
constexpr uint32_t I_load_opcode = 0b000011;
constexpr uint32_t I_jalr_opcode = 0b1100111;
constexpr uint32_t I_syscall_opcode = 0b1110011;   // ecall and ebreak
constexpr uint32_t S_opcode = 0b0100011;
constexpr uint32_t B_opcode = 0b1100011;
constexpr uint32_t Jal_opcode = 0b1101111;
constexpr uint32_t U_lui_opcode = 0b0110111;
constexpr uint32_t U_auipc_opcode = 0b0010111;

class disassembler {
    Memory mem;
    RegisterFile register_file;

    int32_t PC;
    string instruction_name;

    string instruction_name;

    bool is_cur_inst_compressed;

    uint32_t decode_compressed_instruction(uint16_t cinstruction);

    static uint32_t get_opcode(uint32_t instruction);
    static uint32_t get_rd(uint32_t instruction);
    static uint32_t get_rs1(uint32_t instruction);
    static uint32_t get_rs2(uint32_t instruction);
    static uint32_t get_func7(uint32_t instruction);
    static uint32_t get_func3(uint32_t instruction);


    static int32_t  get_imm_itype(uint32_t instruction);
    static int32_t  get_imm_stype(uint32_t instruction);
    static int32_t  get_imm_btype(uint32_t instruction);
    static int32_t  get_imm_utype(uint32_t instruction);
    static int32_t  get_imm_jtype(uint32_t instruction);

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

public:
    //
    explicit disassembler (const std::string& machine_code_fname);

    uint32_t decode_instruction(uint32_t instruction);

    void run();

    void print_string(int32_t address);
};

#endif //RV32IC_ISS_DISASSEMBLER_H
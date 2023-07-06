#include "disassembler.hpp"


disassembler::disassembler(const std::string& machine_code_fname, const std::string& data_section_fname) {

    mem.load_text_section(machine_code_fname);

    //if(data_section_fname != "")
    if(!data_section_fname.empty())
        mem.load_data_section(data_section_fname);
}

void disassembler::run() {
    // Set PC to the beginning of the text section
    // Load half a word per iteration to support compressed instructions

    std::cout << std::setw(2) << std::left << "PC" << "\t" << std::setw(4) << std::left << "Instruction" << "\t" << std::setw(3) << std::left << "Compressed" << "\t" << "Disassembly" << "\n";
    std::cout << "--------------------------------------------------\n";
    for(PC = 0x00000000 ; PC <= mem.get_eot() ; ){
        compressed_instruction = mem.read_half_word(PC);
        if((compressed_instruction & 0x03) != 0x03){
            is_cur_inst_compressed = true ;
            // it is a compressed instruction
            uint32_t word = decode_compressed_instruction(compressed_instruction);
            decode_instruction(word);
            PC+=2;
        }else if((compressed_instruction & 0x03) == 0x03 && (compressed_instruction & 0x1C) != 0x1C) {
            is_cur_inst_compressed = false ;
            uint32_t word = mem.read_word(PC);
            decode_instruction(word);
            PC+=4;
        }else
            continue;
    }
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
    //        000000010100
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

uint32_t disassembler::decode_instruction(uint32_t instruction) {

    uint32_t opcode = get_opcode(instruction);
    switch (opcode) {
        case R_opcode:
            execute_rtype(instruction);
            break;
        case S_opcode:
            execute_stype(instruction);
            break;
        case B_opcode:
            execute_btype(instruction);
            break;
        case I_arth_opcode:
            execute_itype_arit(instruction);
            break;
        case I_load_opcode:
            execute_itype_load(instruction);
            break;
        case I_jalr_opcode:
            execute_itype_jalr(instruction);
            break;
        case I_syscall_opcode:
            execute_ecall();
            break;
        case U_auipc_opcode:
            execute_utype_aui(instruction);
            break;
        case U_lui_opcode:
            execute_utype_lui(instruction);
            break;
        case Jal_opcode:
            execute_jtype(instruction);
            break;
        default:
            break;
    }
    return 0;
}

void disassembler::execute_rtype(uint32_t instruction) {
    uint32_t func3 = get_func3(instruction);
    uint32_t rs1   = get_rs1(instruction);
    uint32_t rs2   = get_rs2(instruction);
    int32_t destination_value = 0x0;

    switch (func3) {
        case 0x0:
            if (get_func7(instruction)) {
                destination_value = register_file.get(rs1) - register_file.get(rs2);
                instruction_name = "SUB";
            } else {
                destination_value = register_file.get(rs1) + register_file.get(rs2);
                instruction_name = "ADD";
            }
            break;
        case 0x1:
            destination_value = register_file.get(rs1) << register_file.get(rs2);
            if (register_file.get(rs2) < 0)
                destination_value = 0;
            instruction_name = "SLL";
            break;
        case 0x2:
            if (register_file.get(rs1) < register_file.get(rs2))
                destination_value = 0x1;
            else
                destination_value = 0x0;
            instruction_name = "SLT";
            break;
        case 0x3:
            if ((uint32_t) register_file.get(rs1) < (uint32_t) register_file.get(rs2))
                destination_value = 0x1;
            else
                destination_value = 0x0;
            instruction_name = "SLTU";
            break;
        case 0x4:
            destination_value = register_file.get(rs1) ^ register_file.get(rs2);
            instruction_name = "XOR";
            break;
        case 0x5:
            if (get_func7(instruction)) {
                destination_value = register_file.get(rs1) >> register_file.get(rs2);
                instruction_name = "SRA";
            } else {
                destination_value = (int32_t) ((uint32_t) register_file.get(rs1) >> register_file.get(rs2));
                instruction_name = "SRL";
            }
            if(register_file.get(rs2)<0)
                destination_value = 0 ;
            break;
        case 0x6:
            destination_value = register_file.get(rs1) | register_file.get(rs2);
            instruction_name = "OR";
            break;
        case 0x7:
            destination_value = register_file.get(rs1) & register_file.get(rs2);
            instruction_name = "AND";
            break;
        default:
            break;
    }
    register_file.set(get_rd(instruction), destination_value);

    std::cout << PC << ":" << "\t" << std::hex << "0x" << std::setw(8) << std::left
    << (!is_cur_inst_compressed ? instruction : compressed_instruction)<<"\t"
    <<std::dec << std::setw(8) << std::left << (is_cur_inst_compressed ? "Yes" : "No") << "\t" <<
    instruction_name << " " << RegisterFile::getMnemonic(get_rd(instruction)) << ","
    << RegisterFile::getMnemonic(rs1) << "," << RegisterFile::getMnemonic(rs2) << "\n";
}

void disassembler::execute_itype_jalr(uint32_t instruction) {
    int32_t destination = get_imm_itype(instruction)+register_file.get(get_rs1(instruction));
    register_file.set(get_rd(instruction),is_cur_inst_compressed ? PC+2 : PC+4);  // MIGHT BE WRONG
    PC = (is_cur_inst_compressed ? destination - 2 : destination - 4);

    instruction_name = "JALR";

    std::cout << PC << ":" << "\t" << std::hex << "0x" << std::setw(8) << std::left << 
    (!is_cur_inst_compressed ? (uint32_t)instruction : (uint16_t)compressed_instruction) << "\t"
    << std::dec << std::setw(8) << std::left << (is_cur_inst_compressed ? "Yes" : "No") << "\t"
    << instruction_name << " " << RegisterFile::getMnemonic(get_rd(instruction)) <<
    "," << RegisterFile::getMnemonic(get_rs1(instruction)) << "," << get_imm_itype(instruction) << "\n";
}


void disassembler::execute_itype_arit(uint32_t instruction) {
    uint32_t func3 = get_func3(instruction);
    uint32_t rs1   = get_rs1(instruction);
    int32_t imm   = get_imm_itype(instruction);
    int32_t destination_value = 0x0;

    switch (func3) {
        case 0x0:
            destination_value = register_file.get(rs1) + imm;
            instruction_name = "ADDI";
            break;
        case 0x1:
            imm = imm & 0x1F;
            destination_value = register_file.get(rs1) << imm ;
            instruction_name = "SLLI";
            break;
        case 0x2:
            if(register_file.get(rs1)<imm)
                destination_value = 0x1;
            else
                destination_value = 0x0;
            instruction_name = "SLTI";
            break;
        case 0x3:
            if((uint32_t)register_file.get(rs1)<(uint32_t)imm)
                destination_value = 0x1;
            else
                destination_value = 0x0;
            instruction_name = "SLTIU";
            break;
        case 0x4:
            destination_value = register_file.get(rs1) ^ imm;
            instruction_name = "XORI";
            break;
        case 0x5:
            if(!(imm & 0xF00)) {
                imm = imm & 0x1F;
                destination_value = (int32_t)((uint32_t)register_file.get(rs1) >> imm);
                instruction_name = "SRLI";
            }else{
                imm = imm & 0x1F;
                destination_value = register_file.get(rs1) >> imm;
                instruction_name = "SRAI";
            }
            break;
        case 0x6:
            destination_value = register_file.get(rs1) | imm;
            instruction_name = "ORI";
            break;
        case 0x7:
            destination_value = register_file.get(rs1) & imm;
            instruction_name = "ANDI";
            break;
        default:
            break;
    }

    register_file.set(get_rd(instruction), destination_value);

    std::cout << PC << ":" << "\t" << std::hex << "0x" << std::setw(8) << std::left
    << (!is_cur_inst_compressed ? (uint32_t)instruction : (uint16_t)compressed_instruction)<< "\t" << std::dec
    << std::setw(8) << std::left << (is_cur_inst_compressed ? "Yes" : "No") << "\t"
    << instruction_name << " " << RegisterFile::getMnemonic(get_rd(instruction))
    << "," << RegisterFile::getMnemonic(rs1) << "," << imm << "\n";
}

void disassembler::execute_stype(uint32_t instruction) {
    uint32_t func3 = get_func3(instruction);
    uint32_t rs2   = get_rs2(instruction);
    int32_t destination_value = get_imm_stype(instruction) + register_file.get(get_rs1(instruction));

    switch (func3) {
        case 0x0:
            mem.store_byte(destination_value,register_file.get(rs2));
            instruction_name = "SB";
            break;
        case 0x1:
            mem.store_half_word(destination_value,register_file.get(rs2));
            instruction_name = "SH";
            break;
        case 0x2:
            mem.store_word(destination_value,register_file.get(rs2));
            instruction_name = "SW";
            break;
        default:
            break;
    }

    std::cout << PC << ":" << "\t" << std::hex << "0x" << std::setw(8) << std::left <<
    (!is_cur_inst_compressed ? (uint32_t)instruction : (uint16_t)compressed_instruction)<<"\t" << std::dec <<
    std::setw(8) << std::left << (is_cur_inst_compressed ? "Yes" : "No") << "\t"
    << instruction_name << " " << RegisterFile::getMnemonic(rs2) << "," <<  get_imm_stype(instruction)
    << "(" << RegisterFile::getMnemonic(get_rs1(instruction)) << ")" << "\n";
}

void disassembler::execute_btype(uint32_t instruction) {
    uint32_t func3 = get_func3(instruction);
    uint32_t rs1   = get_rs1(instruction);
    uint32_t rs2   = get_rs2(instruction);

    switch (func3) {
        case 0x0:
            if(register_file.get(rs1)==register_file.get(rs2))
                PC = (is_cur_inst_compressed ? (PC+get_imm_btype(instruction)-2):(PC+get_imm_btype(instruction)-4)); // MIGHT BE WRONG CHECK LATER
            instruction_name = "BEQ";
            break;
        case 0x1:
            if(register_file.get(rs1)!=register_file.get(rs2))
                PC = (is_cur_inst_compressed ? (PC+get_imm_btype(instruction)-2):(PC+get_imm_btype(instruction)-4)); // MIGHT BE WRONG CHECK LATER
            instruction_name = "BNE";
            break;
        case 0x4:
            if(register_file.get(rs1)<register_file.get(rs2))
                PC = (is_cur_inst_compressed ? (PC+get_imm_btype(instruction)-2):(PC+get_imm_btype(instruction)-4)); // MIGHT BE WRONG CHECK LATER
            instruction_name = "BLT";
            break;
        case 0x5:
            if(register_file.get(rs1)>=register_file.get(rs2))
                PC = (is_cur_inst_compressed ? (PC+get_imm_btype(instruction)-2):(PC+get_imm_btype(instruction)-4)); // MIGHT BE WRONG CHECK LATER
            instruction_name = "BGE";
            break;
        case 0x6:
            if((uint32_t)register_file.get(rs1)<(uint32_t)register_file.get(rs2))
                PC = (is_cur_inst_compressed ? (PC+get_imm_btype(instruction)-2):(PC+get_imm_btype(instruction)-4)); // MIGHT BE WRONG CHECK LATER
            instruction_name = "BLTU";
            break;
        case 0x7:
            if((uint32_t)register_file.get(rs1)>=(uint32_t)register_file.get(rs2))
                PC = (is_cur_inst_compressed ? (PC+get_imm_btype(instruction)-2):(PC+get_imm_btype(instruction)-4)); // MIGHT BE WRONG CHECK LATER
            instruction_name = "BGEU";
            break;
        default:
            break;
    }

    std::cout << PC << ":" << "\t" << std::hex << "0x" << std::setw(8) << std::left 
    <<(!is_cur_inst_compressed ? (uint32_t)instruction : (uint16_t)compressed_instruction)<<"\t"
    <<std::dec << std::setw(8) << std::left << (is_cur_inst_compressed ? "Yes" : "No") << "\t" <<
    instruction_name << " " << RegisterFile::getMnemonic(rs1) << ","
    << RegisterFile::getMnemonic(rs2) << "," << get_imm_btype(instruction) << "\n";
}

void disassembler::execute_jtype(uint32_t instruction) {
    register_file.set(get_rd(instruction),is_cur_inst_compressed ? PC+2 : PC+4);
    PC = (is_cur_inst_compressed ? (PC+get_imm_jtype(instruction)-2):(PC+get_imm_jtype(instruction)-4)); // MIGHT BE WRONG

    instruction_name = "JAL";

    std::cout << PC << ":" <<"\t" << std::hex << "0x" << std::setw(8) << std::left << 
    (!is_cur_inst_compressed ? (uint32_t)instruction : (uint16_t)compressed_instruction )<<"\t"
    <<std::dec << std::setw(8) << std::left << (is_cur_inst_compressed ? "Yes" : "No") << "\t" <<
    instruction_name << " " << RegisterFile::getMnemonic(get_rd(instruction)) << "," << 
    get_imm_jtype(instruction) <<"\n";

}

void disassembler::execute_utype_lui(uint32_t instruction) {
    register_file.set(get_rd(instruction), get_imm_utype(instruction));

    instruction_name = "LUI";

    std::cout << PC << ":" << "\t" << std::hex << "0x" << std::setw(8) << std::left
    << (!is_cur_inst_compressed ? (uint32_t)instruction : (uint16_t)compressed_instruction )<<"\t"
    <<std::dec << std::setw(8) << std::left << (is_cur_inst_compressed ? "Yes" : "No") << "\t"
    << instruction_name << " " << RegisterFile::getMnemonic(get_rd(instruction))
    << "," << get_imm_utype(instruction) << "\n";
}

void disassembler::execute_utype_aui(uint32_t instruction) {
    register_file.set(get_rd(instruction),PC + get_imm_utype(instruction));

    instruction_name = "AUIPC";

    std::cout << PC << ":" << "\t" << std::hex << "0x" << std::setw(8) << std::left << 
    (!is_cur_inst_compressed ? (uint32_t)instruction : (uint16_t)compressed_instruction)<<"\t"
    <<std::dec << std::setw(8) << std::left << (is_cur_inst_compressed ? "Yes" : "No") << "\t"
    << instruction_name << " " << RegisterFile::getMnemonic(get_rd(instruction))
    << "," << get_imm_utype(instruction) << "\n";
}

void disassembler::execute_itype_load(uint32_t instruction) {
    uint32_t func3 = get_func3(instruction);
    uint32_t rs1   = get_rs1(instruction);
    int32_t  imm   = get_imm_itype(instruction);
    int32_t  destination_value = 0x0;

    switch (func3) {
        case 0x0:
            destination_value = (int32_t)mem.read_byte(register_file.get(rs1)+imm);
            instruction_name = "LB";
            break;
        case 0x1:
            destination_value = (int32_t)mem.read_half_word(register_file.get(rs1)+imm);
            instruction_name = "LH";
            break;
        case 0x2:
            destination_value = (int32_t)mem.read_word(register_file.get(rs1)+imm);
            instruction_name = "LW";
            break;
        case 0x4:
            destination_value = (int32_t)((uint32_t)mem.read_byte(register_file.get(rs1)+imm));
            instruction_name = "LBU";
            break;
        case 0x5:
            destination_value = (int32_t)((uint32_t)mem.read_half_word(register_file.get(rs1)+imm));
            instruction_name = "LHU";
            break;
        default:
            break;
    }
    register_file.set(get_rd(instruction),destination_value);

    std::cout << PC << ":" << "\t" << std::hex << "0x" << std::setw(8) << std::left
    << (!is_cur_inst_compressed ? (uint32_t)instruction : (uint16_t)compressed_instruction)<<"\t"
    <<std::dec << std::setw(8) << std::left << (is_cur_inst_compressed ? "Yes" : "No") << "\t"
    << instruction_name << " " <<  RegisterFile::getMnemonic(get_rd(instruction))
    << "," << imm << "(" << RegisterFile::getMnemonic(rs1) << ")" << "\n";
}

void disassembler::execute_ecall() {
    int32_t a7 = register_file.get(17);
    std::cout << PC << ":" << "\t" << "ECALL" << " " << a7 <<"\n";
    switch (a7) {
        case 1:
            std::cout<<"Prints an integer : "<<register_file.get(10)<<"\n";
            break;
        case 4:
            std::cout<<"Prints a string : ";
            print_string(register_file.get(10));
            std::cout<<"\n";
            break;
        case 5:
            int32_t x;
            std::cin >> x;
            register_file.set(10,x);
            break;
        case 10:
            std::cout<<"Exit Program\n";
            exit(0);
        default:
            printf("Error in machine code ecall");
            exit(0);
    }

}

void disassembler::print_string(int32_t address) {
    while(mem.read_byte(address)!=0x0){
        std::cout<<mem.read_byte(address++);
    }
}

/* Decoding Compressed Instructions */
/*
* Decoding Load Instructions
* Supported Instructions: C.LW (lw)
*/
uint32_t disassembler::decode_CL(uint16_t cinstruction) {
    uint32_t dinstruction = 0x0;

    uint32_t rd = (cinstruction & 0x1C) >> 2;
    rd+=8;                                                  // instruction uses x8-x15

    uint32_t imm = (cinstruction & 0x20) >> 4;              // cimm[6] = imm[2]
    imm |= (cinstruction & 0x1C00) >> 7;                    // cimm[10:12] = imm[3:5]
    imm |= (cinstruction & 0x20) << 1;                      // cimm[5] = imm[6]

    uint32_t rs1 = (cinstruction & 0x380) >> 7;
    rs1+=8;                                                 // instruction uses x8-x15

    dinstruction |= I_load_opcode;
    dinstruction |= rd << 7;
    dinstruction |= 2 << 12;                            // func3 = 0x2
    dinstruction |= rs1 << 15;
    dinstruction |= imm << 20;                              // start from imm[2], as it is scaled by 4

    return dinstruction;
}

/*
* Decoding Store Instructions
* Supported Instructions: C.SW (sw)
*/
uint32_t disassembler::decode_CS(uint16_t cinstruction) {
    uint32_t d_instruction = 0x0;

    uint32_t rs2 = (cinstruction & 0x1C) >> 2;
    rs2+=8;                                                 // instruction uses x8-x15


    uint32_t imm = (cinstruction & 0x20) >> 4;              // cimm[6] = imm[2]
    imm |= (cinstruction & 0x1C00) >> 7;                    // cimm[10:12] = imm[3:5]
    imm |= (cinstruction & 0x10) << 1;                      // cimm[5] = imm[6]


    uint32_t rs1 = (cinstruction & 0x380) >> 7;
    rs1+=8;                                                 // instruction uses x8-x15

    d_instruction |= S_opcode;
    d_instruction |= (imm & 0x1F) << 7;                     // move imm[0:4] to dinstruction[7:11]
    d_instruction |= 2 << 12;
    d_instruction |= rs1 << 15;
    d_instruction |= rs2 << 20;
    d_instruction |= (imm & 0x60) << 20;                    // move imm[5:] to dinstruction[25:31]

    return d_instruction;
}

/*
* Decoding Branch Instructions
* Supported Instructions: beqz (beq r, x0, label), bnez (bne r, x0, label)
*/
uint32_t disassembler::decode_CB_B(uint16_t cinstruction, uint32_t func3) {
    uint32_t d_instruction = 0x0;

    uint32_t rs1 = (cinstruction & 0x380) >> 7;
    rs1+=8;                                                 // instruction uses x8-x15

    // Typecasting to int32_2 then shifting performs sign extension for us automatically
    int32_t imm = (cinstruction & 0x18) >> 2;               // cimm[3:4] = imm[1:2]
    imm |= (int32_t)(cinstruction & 0xC00) >> 7;            // cimm[10:11] = imm[3:4]
    imm |= (int32_t)(cinstruction & 0x4) << 3;              // cimm[2] = imm[5]
    imm |= (int32_t)(cinstruction & 0x60) << 1;             // cimm[5:6] = imm[6:7]
    imm |= (int32_t)(cinstruction & 0x1000) >> 4;            // cimm[12] = imm[8]
    imm = (imm << 23) >> 23;

    imm = imm >> 1;


    d_instruction |= B_opcode;
    d_instruction |= (imm & 0x800) >> 4;                     // imm[11]
    d_instruction |= (imm & 0xF) << 8;                  // imm[4:1]
    d_instruction |= func3 << 12;                       // func3 of beq
    d_instruction |= rs1 << 15;
    // leave rs2 as zero
    d_instruction |= (imm & 0x3F0) << 21;           // imm[5:10]
    d_instruction |= (imm & 0x800) << 20;


    return d_instruction;
}

/*
* Decoding Branch-Immediate Instructions (immediate instructions that are encoded in CB format)
* Supported Instructions: srli, srai, andi
*/

uint32_t disassembler::decode_CB_I(uint16_t cinstruction, uint32_t func3){
    uint32_t d_instruction = 0x0;

    uint32_t rs1 = (cinstruction & 0x380) >> 7;
    rs1+=8;                                                 // instruction uses x8-x15

    // Typecasting to int32_2 then shifting performs sign extension for us automatically
    int32_t imm = (cinstruction & 0x7C) >> 2;                // cimm[2:6] = imm[0:4]
    imm |= (int32_t)(cinstruction & 0x1000) >> 7;            // cimm[12] = imm[5]
    imm = (imm << 26) >> 26;


    d_instruction |= I_arth_opcode;
    d_instruction |= (rs1 << 7);                              // rd = rs1
    d_instruction |= (func3 << 12);
    d_instruction |= (rs1 << 15);
    d_instruction |= uint32_t((int32_t)(imm << 20));

    if(((cinstruction & 0x400) >> 10) == 0x01)
        d_instruction |= 0b0100000 << 25;                        // srai
    return d_instruction;
}

/*
* Decoding Stack-Pointer Relative Store
* Supported Instructions: SWSP (sw rs2, offset[2:7](sp))
*/

uint32_t disassembler::decode_CSS(uint16_t cinstruction) {
    uint32_t d_dinstruction = 0x0;

    uint32_t rs2 = (cinstruction & 0x7C) >> 2;

    uint32_t imm = (cinstruction & 0x1E00) >> 7;        // cimm[9:12] = imm[2:5]
    imm |= (cinstruction & 0x180) >> 1;                 // cimm[7:8] = imm[6:7]

    d_dinstruction |= S_opcode;
    d_dinstruction |= (imm & 0x1C) << 7;
    d_dinstruction |= 2 << 12;
    d_dinstruction |=  0x2 << 15;                       // sp
    d_dinstruction |= rs2 << 20;
    d_dinstruction |= (imm & 0x0E0) << 20;

    return d_dinstruction;
}

/*
* Decoding Immediate-Wide Instructions
* Supported Instructions: addi4spn (addi rd, sp, nzuimm[9:2])
* nzuimm: zero-extended unsigned immediate (!= 0)
*/

uint32_t disassembler::decode_CIW(uint16_t cinstruction) {
    uint32_t d_instruction = 0x0;

    uint32_t rd = (cinstruction & 0x1C) >> 2;
    rd+=8;                                                  // instruction uses x8-x15

    uint32_t imm = (cinstruction & 0x40) >> 4;       // cimm[6] = imm[2]
    imm |= (cinstruction & 0x20) >> 2;               // cimm[5] = imm[3]
    imm |= (cinstruction & 0x1800) >> 7;             // cimm[11:12] = imm[4:5]]
    imm |= (cinstruction & 0x780) >> 1;             // cimm[7:10] = imm[6:9]

    d_instruction |= I_arth_opcode;
    d_instruction |= rd << 7;
    d_instruction |= 0 << 12;
    d_instruction |= 0x2 << 15;
    d_instruction |= imm << 20;

    return d_instruction;
}

/*
* Decoding Jump instructions
* Supported Instructions: j (jal x0, offset[1:11]), jal x1, offset[11:1]
* rd is not encoded in the compressed format, thus passing rd
*/

uint32_t disassembler::decode_CJ(uint16_t cinstruction, uint32_t rd){

    uint32_t d_instruction = 0x0;
    int32_t imm ;
    imm = (int32_t)((cinstruction & 0x1000));
    imm = (imm << 19) >> 19 ;
    imm |= (int32_t)(cinstruction & 0x38) >> 2;             // cimm[3:5] = imm[1:3]
    imm |= (int32_t)(cinstruction & 0x800) >> 7;            // cim[11] = imm[4]
    imm |= (int32_t)(cinstruction & 0x4) << 3;              // cimm[2] = imm[5]
    imm |= (int32_t)(cinstruction & 0x80) >> 1;             // cimm[7] = imm[6]
    imm |= (int32_t)(cinstruction & 0x40) << 1;             // cimm[6] = imm[7]
    imm |= (int32_t)(cinstruction & 0x600) >> 1;            // cimm[9:10] = imm[8:9]
    imm |= (int32_t)(cinstruction & 0x100) << 2;            // cimm[8] = imm[10]
    imm |= (int32_t)(cinstruction & 0x1000) >> 1;           // cimm[12] = imm[11]
    imm = (imm << 20) >> 20;
    imm = imm >> 1 ;

    d_instruction |=  Jal_opcode;
    d_instruction |= rd << 7;
    d_instruction |= (imm & 0x400) << 10;                 // imm[11]
    d_instruction |= (imm & 0x3FF) << 21;
    d_instruction |= (imm & 0xFF000);
    d_instruction |= (imm & 0x400) << 21;                  // imm[12]

    return d_instruction;
}

/*
* Decoding Arithmetic Instructions
* Supported Instructions: and, or, xor, sub
*/

uint32_t disassembler::decode_CA(uint16_t cinstruction, uint32_t func3){
    uint32_t d_instruction = 0x0;

    uint32_t rd = (cinstruction & 0x380) >> 7;
    rd+=8;                                                  // instruction uses x8-x15
    uint32_t rs2 = (cinstruction & 0x1C) >> 2;
    rs2+=8;                                                 // instruction uses x8-x15
    uint32_t func7 = (func3 == 0x0) ? 0x20: 0x00;

    d_instruction |= R_opcode;
    d_instruction |= rd << 7;
    d_instruction |= func3 << 12;
    d_instruction |= rd << 15;
    d_instruction |= rs2 << 20;
    d_instruction |= func7  << 25;

    return d_instruction;
}

/**
* Decoding Jump and Link Register Instructions
* Supported Instructions: jalr (jalr x1, 0(rs1)), jr(jalr 0, 0(rs1))
*/
uint32_t disassembler::decode_CR_Jar(uint16_t cinstruction, uint32_t rd){
    uint32_t d_instruction = 0x0;

    uint32_t rs1 = (cinstruction & 0xF80)>>7;
    uint32_t func3 = 0x0;                                   // func3 for jalr
    uint32_t imm = 0x0;

    d_instruction |= I_jalr_opcode;
    d_instruction |= rd << 7;
    d_instruction |= func3 << 12;
    d_instruction |= rs1 << 15;
    d_instruction |= imm << 20;

    return d_instruction;
}

/*
* Decoding some Register Instructions
* Supported Instructions: add, mv
*/

uint32_t disassembler::decode_CR_ADD(uint16_t cinstruction){
    uint32_t d_instruction = 0x0;

    uint32_t rs2 = (cinstruction & 0x7C)>>2;
    uint32_t rd = (cinstruction & 0xF80)>>7;
    uint32_t func3 = 0x0;
    uint32_t func7 = 0x0;

    d_instruction |= R_opcode;
    d_instruction |= rd << 7;
    d_instruction |= func3 << 12;
    // leave rs1 as zero for mv
    if(((cinstruction & 0x1000) >> 12) == 0x1)
        d_instruction |= rd << 15;

    d_instruction |= rs2 << 20;
    d_instruction |= func7 << 25;

    return d_instruction;
}

/**
* Decoding Arithmetic Immediate Instructions
* Supported Instructions: addi, slli, nop, li
*/
uint32_t disassembler::decode_CI_I(uint16_t cinstruction) {
uint32_t d_instruction = 0x0;

uint32_t rd = (cinstruction & 0xF80) >> 7;
int32_t imm = (cinstruction & 0x7C) >> 2;  //imm[4:0]
imm |= (int32_t)(cinstruction & 0x1000) >> 7; //imm[5]
imm = (imm << 26) >> 26;

d_instruction |= I_arth_opcode;
d_instruction |= rd << 7;
d_instruction |= 0 << 12;

// leave rs1 as zero for li
if(((cinstruction & 0x4000) >> 14) != 0x1)
    d_instruction |= rd << 15;

d_instruction |= (imm <<  20);

return d_instruction;
}

/**
* Decoding Load Stack Pointer Relative Instructions
* Supported Instructions: lwsp (lw rd, offset[2:7](sp))
*/
uint32_t disassembler::decode_CI_LWSP(uint16_t cinstruction){
    uint32_t d_instruction = 0x0;

    uint32_t rs1 = 0x2; //always x2
    uint32_t rd = (cinstruction & 0xF80) >> 7;
    uint32_t func3 = 0x2;

    int32_t imm = (cinstruction & 0x70) >> 2;           //imm[4:2]
    imm |= (int32_t)(cinstruction & 0x1000) >> 7;       //imm[5]
    imm |= (int32_t)(cinstruction & 0xC) << 4;          //imm[7:6]
    imm = (imm << 24) >> 24;

    d_instruction |= I_load_opcode;
    d_instruction |= rd << 7;
    d_instruction |= func3 << 12;
    d_instruction |= rs1 << 15;
    d_instruction |= (imm <<  20);
    return d_instruction;
}

uint32_t disassembler::decode_ADDI16SP(uint16_t cinstruction){
    uint32_t d_instruction = 0x0;

    uint32_t rd = 0x2;     //always x2
    uint32_t func3 = 0x0;

    int32_t imm = (cinstruction & 0x40) >> 2;           //imm[4]
    imm |= (int32_t)(cinstruction & 0x4) << 3;          //imm[5]
    imm |= (int32_t)(cinstruction & 0x20) << 1;          //imm[6]
    imm |= (int32_t)(cinstruction & 0x18) << 4;         //imm[7:8]
    imm |= (int32_t)(cinstruction & 0x1000) >> 3;       //imm[9]
    imm = (imm << 22) >> 22;

    d_instruction |= I_arth_opcode;
    d_instruction |= rd << 7;
    d_instruction |= func3 << 12;
    d_instruction |= rd << 15;
    d_instruction |= (imm <<  20);
    return d_instruction;
}

uint32_t disassembler::decode_CI_LUI(uint16_t cinstruction){
    uint32_t d_instruction = 0x0;

    uint32_t rd = (cinstruction & 0xF80) >> 7;
    int32_t imm = (int32_t)(cinstruction & 0x7C) << 10;         // cimm [2:6] = nzimm[12:16]
    imm |= (int32_t)(cinstruction & 0x1000) << 4;               // cimm[12] = nzimm[17]
    imm = (imm << 14) >> 14;
    imm = imm >> 12;

    d_instruction |= U_lui_opcode;
    d_instruction |= rd << 7;
    d_instruction |= (imm << 12);

    return d_instruction;
}

uint32_t disassembler::decode_compressed_instruction(uint16_t cinstruction){
    uint32_t opcode = get_C_opcode(cinstruction);
    uint32_t func3 = get_C_func3(cinstruction);
    uint32_t instruction = 0x0;

    switch (opcode)
    {
        case 0x0:{
            switch (func3)
            {
                case 0x0:
                    // C.ADDI4SPN
                    instruction = decode_CIW(cinstruction);
                    break;
                case 0x2:
                    // C.LW
                    instruction = decode_CL(cinstruction);
                    break;
                case 0x6:
                    // C.SW
                    instruction = decode_CS(cinstruction);
                    break;
                default:
                    std::cout << "Unknown Compressed Instruction\n";
                    break;
                    // exit(0);
            }
            break;
        }
        case 0x1:{
            switch(func3)
            {
                case 0x0:
                case 0x2:
                    // C.ADDI & C.NOP & C.LI
                    instruction = decode_CI_I(cinstruction);
                    break;
                case 0x1:
                    // C.JAL
                    instruction = decode_CJ(cinstruction, 0x1);
                    break;
                case 0x3:
                    // C.LUI & C.ADDI16SP
                    if(((cinstruction & 0xF80) >> 7) == 0x2)
                        instruction = decode_ADDI16SP(cinstruction);
                    else
                        instruction = decode_CI_LUI(cinstruction);
                    break;
                case 0x4:{
                    uint32_t helping_func3 = (cinstruction & 0xC00) >> 10; // help differenitate instructions with opcode 4
                    uint32_t helping_func7 = (cinstruction & 0x60) >> 5; // help differentiate instructions with opcode 4 belonging to CA

                    if(helping_func3 == 0x0 || helping_func3 == 0x1)
                        // C.SRLI & C.SRAI
                        instruction = decode_CB_I(cinstruction, 0x5);
                    else if(helping_func3 == 0x2)
                        // C.ANDI
                        instruction = decode_CB_I(cinstruction, 0x7);
                    else{
                        if(helping_func7 == 0x0)
                            // C.SUB
                            instruction = decode_CA(cinstruction, 0x0);
                        else if(helping_func7 == 0x1)
                            // C.XOR
                            instruction = decode_CA(cinstruction, 0x4);
                        else if(helping_func7 == 0x2)
                            // C.OR
                            instruction = decode_CA(cinstruction, 0x6);
                        else{
                            // C.AND
                            instruction = decode_CA(cinstruction, 0x7);
                        }
                    }
                    break;
                }
                case 0x5:
                    // C.J
                    instruction = decode_CJ(cinstruction, 0x0);
                    break;
                case 0x6:
                    // C.BEQZ
                    instruction = decode_CB_B(cinstruction, 0x0);
                    break;
                case 0x7:
                    // C.BNEZ
                    instruction = decode_CB_B(cinstruction, 0x1);
                    break;
                default:
                    std::cout << "Unknown Compressed Instruction\n";
                    break;
                    // exit(0);
            }
            break;
        }
        case 0x2:{
            switch(func3)
            {
                case 0x0:
                    // C.SLLI
                    instruction = decode_CB_I(cinstruction, 0x1);
                    break;
                case 0x2:
                    // C.LWSP
                    instruction = decode_CI_LWSP(cinstruction);
                    break;
                case 0x4:{
                    uint32_t rs1 = (cinstruction & 0x780) >> 7;
                    uint32_t rs2 = (cinstruction & 0x7C) >> 2;
                    uint32_t helping_bit = (cinstruction & 0x1000) >> 12; // 13th bit

                    if(rs1 != 0 && rs2 == 0 && helping_bit == 0)
                        // C.JR
                        instruction = decode_CR_Jar(cinstruction, 0x0);
                    else if(rs1 != 0 && rs2 == 0 && helping_bit == 1)
                        // C.JALR
                        instruction = decode_CR_Jar(cinstruction, 0x1);
                    else if(rs1 == 0 && rs2 == 0);
                        // C.EBREAK
                    else if(rs1 != 0 && rs2 != 0)
                        // C.MV & C.ADD
                        instruction = decode_CR_ADD(cinstruction);
                    else{
                        std::cout << "Unknown Compressed Instruction\n";
                        // exit(0);
                    }
                    break;
                }
                case 0x6:
                    instruction = decode_CSS(cinstruction);
                    break;
                default:
                    std::cout << "Unknown Compressed Instruction\n";
                    break;
                    // exit(0);
            }
            break;
        }
        default:
            std::cout << "Unknown Compressed Instruction\n";
            // exit(0);
            // End of opcode switch
    }
    return instruction;
}
uint32_t disassembler::get_C_opcode(uint16_t cinstruction) {
    // Return 0 - 1 bits
    return (cinstruction & 0x03);
}

uint32_t disassembler::get_C_func3(uint16_t cinstruction) {
    // Return 13 - 15 bits
    return (cinstruction & 0xE000) >> 13;
}
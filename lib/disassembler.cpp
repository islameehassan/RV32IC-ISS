#include "disassembler.h"

disassembler::disassembler(const std::string& machine_code_fname) {
    mem.load_text_section(machine_code_fname);
    run();
    // decode_instruction(65815);
    /* testing
    mem.store_byte(0,0b01101000);
    mem.store_byte(1,0b01100101);
    mem.store_byte(2,0b01101100);
    mem.store_byte(3,0b01101100);
    mem.store_byte(4,0b00000000);
    print_string(0x00000004);
    register_file.set(31,10);
    register_file.set(1,0);
    PC = 4 ;
    uint32_t instruction = 0xfe2f80e7;
    execute_itype_jalr(instruction);
    int32_t test = register_file.get(1);
    int32_t test2 = register_file.get(31);
    std::cout<<test<<"\n"<<test2<<"\n"<<PC;
     */
}

void disassembler::run() {
    // Set PC to the beginning of the text section
    // Load half a word per iteration to support compressed instructions
    for(PC = 0x00000000 ; PC <= mem.get_eot() ; ){
        uint16_t  half_word = mem.read_half_word(PC);
        if((half_word & 0x03) != 0x03){
            is_cur_inst_compressed = true ;
            // it is a compressed instruction
            std::cout<<"ERROR"<<PC<<"\n";
            PC+=2;
        }else if((half_word & 0x03) == 0x03 && (half_word & 0x1C) != 0x1C) {
            is_cur_inst_compressed = false ;
            uint32_t word = mem.read_word(PC);
            decode_instruction(word);
            PC+=4;
        }else
            continue;
    }
}

uint32_t disassembler::decode_compressed_instruction(uint16_t cinstruction) {
    return 0;
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
<<<<<<< HEAD
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
=======
            else
                destination_value = register_file.get(rs1) + register_file.get(rs2);
                instruction_name = "ADD";
            break;
        case 0x1:
            destination_value = register_file.get(rs1) << register_file.get(rs2);
            if(register_file.get(rs2)<0)
                destination_value = 0 ;
                instruction_name = "SLL";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
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
<<<<<<< HEAD
            destination_value = register_file.get(rs1) ^ register_file.get(rs2);
            instruction_name = "XOR";
=======
                destination_value = register_file.get(rs1) ^ register_file.get(rs2);
                instruction_name = "XOR";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
            break;
        case 0x5:
            if (get_func7(instruction)) {
                destination_value = register_file.get(rs1) >> register_file.get(rs2);
                instruction_name = "SRA";
<<<<<<< HEAD
            } else {
                destination_value = (int32_t) ((uint32_t) register_file.get(rs1) >> register_file.get(rs2));
                instruction_name = "SRL";
            }
=======
            else
                destination_value = (int32_t)((uint32_t)register_file.get(rs1) >> register_file.get(rs2));
                instruction_name = "SRL";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
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
        register_file.set(get_rd(instruction), destination_value);
        std::cout << instruction_name << std::setw(8) << register_file.getMnemonic(get_rd(instruction)) << std::setw(8)
            << register_file.getMnemonic(rs1) << std::setw(8) << register_file.getMnemonic(rs2) << "\n";
        
    }
<<<<<<< HEAD
    register_file.set(get_rd(instruction), destination_value);
    std::cout << instruction_name << std::setw(8) << register_file.getMnemonic(get_rd(instruction)) << std::setw(8)
              << register_file.getMnemonic(rs1) << std::setw(8) << register_file.getMnemonic(rs2) << "\n";

=======
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
}

void disassembler::execute_itype_jalr(uint32_t instruction) {
    int32_t destination = get_imm_itype(instruction)+register_file.get(get_rs1(instruction));
    int32_t imm = get_imm_itype(instruction);
    uint32_t rs1 = get_rs1(instruction);
    int32_t tmp = register_file.get(rs1);
    register_file.set(get_rd(instruction),is_cur_inst_compressed ? PC+2 : PC+4);  // MIGHT BE WRONG
    PC = (is_cur_inst_compressed ? destination - 2 : destination - 4);
    instruction_name = "JALR";
<<<<<<< HEAD
    std::cout << instruction_name << std::setw(8) << register_file.getMnemonic(get_rd(instruction)) << "\n";
=======
    cout << instruction_name << std::setw(8) << register_file.getMnemonic(get_rd(instruction)) << "\n";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
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
    std::cout << instruction_name << std::setw(8) << register_file.getMnemonic(get_rd(instruction)) << std::setw(8)
<<<<<<< HEAD
              << register_file.getMnemonic(rs1) << std::setw(8) << std::hex << imm << "\n";


=======
        << register_file.getMnemonic(rs1) << std::setw(8) << std::hex << imm << "\n";

    
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
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
<<<<<<< HEAD
    std::cout << instruction_name << std::setw(8)
              << register_file.getMnemonic(rs2) << std::setw(8) << std::hex << get_imm_stype(instruction)
              << "(" << register_file.getMnemonic(get_rd(instruction)) << ")" << "\n";
=======
    std::cout << instruction_name << std::setw(8) << std::hex << get_imm_stype(instruction) << "(" << register_file.getMnemonic(get_rd(instruction)) << ")" << std::setw(8)
        << register_file.getMnemonic(rs2) << "\n";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29

}

void disassembler::execute_btype(uint32_t instruction) {
    uint32_t func3 = get_func3(instruction);
    uint32_t rs1   = get_rs1(instruction);
    uint32_t rs2   = get_rs2(instruction);

    switch (func3) {
        case 0x0:
            if(register_file.get(rs1)==register_file.get(rs2))
                PC = (is_cur_inst_compressed ? (PC+get_imm_btype(instruction)-2):(PC+get_imm_btype(instruction)-4)); // MIGHT BE WRONG CHECK LATER
<<<<<<< HEAD
            instruction_name = "BEQ";
=======
                instruction_name = "BEQ";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
            break;
        case 0x1:
            if(register_file.get(rs1)!=register_file.get(rs2))
                PC = (is_cur_inst_compressed ? (PC+get_imm_btype(instruction)-2):(PC+get_imm_btype(instruction)-4)); // MIGHT BE WRONG CHECK LATER
<<<<<<< HEAD
            instruction_name = "BNE";
=======
                instruction_name = "BNE";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
            break;
        case 0x4:
            if(register_file.get(rs1)<register_file.get(rs2))
                PC = (is_cur_inst_compressed ? (PC+get_imm_btype(instruction)-2):(PC+get_imm_btype(instruction)-4)); // MIGHT BE WRONG CHECK LATER
<<<<<<< HEAD
            instruction_name = "BLT";
=======
                instruction_name = "BLT";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
            break;
        case 0x5:
            if(register_file.get(rs1)>=register_file.get(rs2))
                PC = (is_cur_inst_compressed ? (PC+get_imm_btype(instruction)-2):(PC+get_imm_btype(instruction)-4)); // MIGHT BE WRONG CHECK LATER
<<<<<<< HEAD
            instruction_name = "BGE";
=======
                instruction_name = "BGE";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
            break;
        case 0x6:
            if((uint32_t)register_file.get(rs1)<(uint32_t)register_file.get(rs2))
                PC = (is_cur_inst_compressed ? (PC+get_imm_btype(instruction)-2):(PC+get_imm_btype(instruction)-4)); // MIGHT BE WRONG CHECK LATER
<<<<<<< HEAD
            instruction_name = "BLTU";
=======
                instruction_name = "BLTU";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
            break;
        case 0x7:
            if((uint32_t)register_file.get(rs1)>=(uint32_t)register_file.get(rs2))
                PC = (is_cur_inst_compressed ? (PC+get_imm_btype(instruction)-2):(PC+get_imm_btype(instruction)-4)); // MIGHT BE WRONG CHECK LATER
<<<<<<< HEAD
            instruction_name = "BGEU";
=======
                instruction_name = "BGEU";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
            break;
        default:
            break;
            std::cout << instruction_name << std::setw(8) << register_file.getMnemonic(rs1) << std::setw(8)
                << register_file.getMnemonic(rs2) << std::setw(8) << std::hex << get_imm_btype(instruction) << "\n";     //need checking <<<<<<<<<<<<<<<<<<

    }
    std::cout << instruction_name << std::setw(8) << register_file.getMnemonic(rs1) << std::setw(8)
              << register_file.getMnemonic(rs2) << std::setw(8) << std::hex << get_imm_btype(instruction) << "\n";     //need checking <<<<<<<<<<<<<<<<<<

}

void disassembler::execute_jtype(uint32_t instruction) {
    register_file.set(get_rd(instruction),is_cur_inst_compressed ? PC+2 : PC+4);
    PC = (is_cur_inst_compressed ? (PC+get_imm_jtype(instruction)-2):(PC+get_imm_jtype(instruction)-4)); // MIGHT BE WRONG
<<<<<<< HEAD

    instruction_name = "JAL";

    std::cout << instruction_name << std::setw(8) << register_file.getMnemonic(get_rd(instruction)) << std::setw(8)
              << std::hex << get_imm_jtype(instruction) <<"\n";
=======
    instruction_name = "JAL";
    std::cout << instruction_name << std::setw(8) << register_file.getMnemonic(get_rd(instruction)) << std::setw(8)
       << std::hex << (is_cur_inst_compressed ? (get_imm_jtype(instruction) - 2) : (get_imm_jtype(instruction) - 4))<< "\n";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29

}

void disassembler::execute_utype_lui(uint32_t instruction) {
    register_file.set(get_rd(instruction), get_imm_utype(instruction));
<<<<<<< HEAD

    instruction_name = "LUI";

    std::cout << instruction_name << std::setw(8) << register_file.getMnemonic(get_rd(instruction)) << std::setw(8)
              << std::hex << get_imm_utype(instruction) << "\n";
=======
    instruction_name = "LUI";
    std::cout << instruction_name << std::setw(8) << register_file.getMnemonic(get_rd(instruction)) << std::setw(8)
        << std::hex << get_imm_utype(instruction) << "\n";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
}

void disassembler::execute_utype_aui(uint32_t instruction) {
    register_file.set(get_rd(instruction),PC + get_imm_jtype(instruction));
    instruction_name = "AUIPC";
    std::cout << instruction_name << std::setw(8) << register_file.getMnemonic(get_rd(instruction)) << std::setw(8)
<<<<<<< HEAD
              << std::hex << get_imm_jtype(instruction) << "\n";
=======
        << std::hex << get_imm_jtype(instruction) << "\n";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
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
<<<<<<< HEAD

    std::cout << instruction_name << std::setw(8) << register_file.getMnemonic(rs1)
    << std::setw(8) << std::hex << imm <<std::setw(8)
    << "(" << register_file.getMnemonic(get_rd(instruction)) << ")" << std::setw(8) << "\n";
=======
    std::cout << instruction_name << std::setw(8) << register_file.getMnemonic(rs1) << std::hex << imm << "(" << 
        register_file.getMnemonic(get_rd(instruction)) << ")" << std::setw(8) << "\n";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
}

void disassembler::execute_ecall() {
    int32_t a7 = register_file.get(17);
    switch (a7) {
        case 1:
            std::cout<<register_file.get(10)<<"---\n";
            break;
        case 4:
            print_string(register_file.get(10));
            break;
        case 5:
            int32_t x;
            std::cin >> x;
            register_file.set(10,x);
            break;
        case 10:
            exit(0);
        default:
            printf("Error in machine code ecall");
            exit(0);
    }
<<<<<<< HEAD

=======
    instruction_name = "ECALL";
    std::cout << instruction_name << "\n";
>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29
}

void disassembler::print_string(int32_t address) {
    while(mem.read_byte(address)!=0x0){
        std::cout<<mem.read_byte(address++);
    }
}

<<<<<<< HEAD
=======
void disassembler::print_instruction() {

}


>>>>>>> 1dc590403b05f61715d01264a55d9553eaffbb29


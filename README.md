# <p align=center> RV32IC-ISS </p>
### <p align=center> Ahmed Elbarbary, Ziad Hassan, Islam Mahdy, Salma Khalil
### <p align=center> Instructor: Mohamed Shalaan
### <p align=center> The American Univesity in Cairo 
## Introduction
<font size = "12">
Welcome to the RV32IC-ISS GitHub repository! This project aims to provide a powerful and efficient solution for converting machine code into RISC-V assembly instructions. The disassembler takes raw machine code as input and produces the corresponding assembly instructions, allowing developers to analyze and understand low-level code.

## How to build the project?
## Project Features
<font size = "12">
The Disassembler project provides the following key features:

Machine Code to Assembly Conversion: The core functionality of this disassembler is its ability to convert machine code instructions into their corresponding RISC-V assembly instructions. It performs a thorough analysis of the binary code, decoding it and producing human-readable assembly output, then prints it into the compiler.

Execute Machine Code: In addition to converting machine code to assembly, this code executes the machine code directly.

Extensibility: The project is built with extensibility in mind, making it easy to add support for additional machine code formats and customizations. Whether you want to incorporate new instruction sets or enhance the disassembler's functionality, you'll find a modular architecture that simplifies the process. For example, there are functions to extract operation code, set the register value, and load it.

## Input Format
<font size = "12">
The input of the disassembler program is the machine code that you want to convert into RISC-V assembly instructions. Machine code consists of binary instructions that are directly executable by the computer's hardware. The disassembler takes this machine code as input and performs the necessary decoding and analysis to convert it into human-readable assembly instructions.
The following is a list of instruction formats:

![Screenshot 2023-07-06 004351](https://github.com/islameehassan/RV32IC-ISS/assets/96207250/707bfe62-88c3-425c-ae8c-858a11c1ff70)
# The instructions that this code can execute and decode are the following:
<font size = "10">

![Screenshot 2023-07-06 005618](https://github.com/islameehassan/RV32IC-ISS/assets/96207250/1baa3f57-88b4-4992-81ad-c78b6c56eba4)
![Screenshot 2023-07-06 005749](https://github.com/islameehassan/RV32IC-ISS/assets/96207250/7c84df52-e837-4930-a4f5-02e7745ee916)
![Screenshot 2023-07-06 005942](https://github.com/islameehassan/RV32IC-ISS/assets/96207250/2d9027e9-0663-48c2-84ac-25bfdb5ec2de)
![Screenshot 2023-07-06 011159](https://github.com/islameehassan/RV32IC-ISS/assets/96207250/734865a7-dfdd-46ed-a550-730c9d9c5a40)
![image](https://github.com/islameehassan/RV32IC-ISS/assets/96207250/689b1279-8e1d-468e-a666-5d51c6481c89)

# Data Storage and Representation
Arrays serve as the main data structure for representing both the memory and the register file in the  RISC-V processor simulation. The code also includes standard C++ data structures nt, uint32_t, and std::string. These are essential for representing integers, register numbers and strings.

# Disassembler Class

The __'disassembler'__ class encapsulates the core functionality of the disassembler. It contains methods for decoding machine code instructions, executing instructions, and printing the corresponding assembly instructions. Some key methods of this class include:

__'execute_rtype'__: Executes R-type instructions (e.g., add, sub, and, or, etc.).<br />
__'execute_stype'__: Executes S-type instructions (e.g., store).<br />
__'execute_btype'__: Executes B-type instructions (e.g., branch).<br />
__'execute_jtype'__: Executes J-type instructions (e.g., jump).<br />
__'execute_ecall'__: Executes ecall and ebreak instructions.<br />
__'execute_utype_lui'__: Executes LUI instructions.<br />
__'execute_utype_aui'__: Executes AUIPC instructions.<br />
__'execute_itype_arit'__: Executes I-type arithmetic instructions (e.g., addi, xori, etc.).<br />
__'execute_itype_load'__: Executes I-type load instructions (e.g., lw, lb, etc.).<br />
__'execute_itype_jalr'__: Executes I-type jalr instruction.<br />
The __'disassembler'__ class also includes various methods for decoding compressed instructions and extracting immediate values from instructions.<br />

# Memory Class

The memory class represents the simulated memory of the CPU. It handles read and write operations to the memory, as well as loading the machine code sections. The memory is byte-addressable, and it provides methods such as __'read_byte', 'read_half_word', 'read_word', 'store_byte', 'store_half_word', and 'store_word'__ for accessing and modifying memory contents.

# Register File Class

The __'register file'__ class handles read and write operations to the CPU's register file. It provides methods for setting the value of a register, getting the value stored in a register, and retrieving the mnemonic name of a register.

# Conclusion

The Disassembler project offers a powerful solution for converting machine code into readable RISC-V assembly instructions. With its efficient decoding and analysis capabilities, developers can gain insights into program behavior, debug code, and optimize execution.

The project consists of three main classes: disassembler, memory, and register file. The disassembler class handles instruction decoding, execution, and printing of assembly instructions. The memory class manages memory operations, while the register file class handles register read and write operations.

Supporting various input formats, the disassembler integrates smoothly into different workflows, accommodating different machine code sources.

The project's documentation provides clear instructions and examples for easy implementation. Contributions from the developer community are encouraged to enhance the disassembler's functionality.

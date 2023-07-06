/*
* Handles write and read to the simulated memory
* Data Storage : Little endian
* 64Kbytes for text section
* 16Kbytes for data section
* Any changes in memory sizes requires changing all memory functions
*/

#include "memory.hpp"

Memory::Memory() {
    // Initialize memory with 0x0 and end of text section with 0
    eof_text_section = 0;
}

uint8_t Memory::read_byte(int location) {
    // undefined access to memory
    try{
    if(location < 0x00000000 || location >= 0x00014000)
        throw std::out_of_range("memory accessing out of range");
    } catch (const std::out_of_range& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    // return byte at location
    return memory_array[location];
}

uint16_t Memory::read_half_word(int location) {
    // undefined access to memory
    try{
        if(location < 0x00000000 || location >= 0x00014000)
            throw std::out_of_range("memory accessing out of range");
    } catch (const std::out_of_range& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    try{
        if(location>=0x00013FFF)
            throw std::overflow_error("memory overflow");
    } catch (const std::overflow_error &o) {
        std::cout << "Exception caught: " << o.what() << std::endl;
    }
    // return half word starting from location
    return (256U* read_byte(location+1))+ read_byte(location);
}

uint32_t Memory::read_word(int location) {
    // undefined access to memory
    try{
        if(location < 0x00000000 || location >= 0x00014000)
            throw std::out_of_range("memory accessing out of range");
    } catch (const std::out_of_range& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    try{
        if(location>=0x00013FFD)
            throw std::overflow_error("memory overflow");
    } catch (const std::overflow_error &o) {
        std::cout << "Exception caught: " << o.what() << std::endl;
    }
    // return word starting from location
    return (16777216U* read_byte(location+3)+(65536U*read_byte(location+2))
           +(256U*read_byte(location+1))+read_byte(location));
}

int Memory::store_byte(int location, uint32_t word) {
    // undefined access to memory
    try{
        if(location < 0x00000000 || location >= 0x00014000)
            throw std::out_of_range("memory accessing out of range");
    } catch (const std::out_of_range& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    // store byte in location
    memory_array[location] = word & 0x000000FF;
    return 0;
}

int Memory::store_half_word(int location, uint32_t word) {
    // undefined access to memory
    try{
        if(location>=0x00013FFF)
            throw std::overflow_error("memory overflow");
    } catch (const std::overflow_error &o) {
        std::cout << "Exception caught: " << o.what() << std::endl;
    }
    // store half word starting from location
    store_byte(location, 0x000000FF & word);
    store_byte(location+1, (0x0000FF00 & word)>>8);
    return 0;
}

int Memory::store_word(int location, uint32_t word) {
    // undefined access to memory
    try{
        if(location>=0x00013FFD)
            throw std::overflow_error("memory overflow");
    } catch (const std::overflow_error &o) {
        std::cout << "Exception caught: " << o.what() << std::endl;
    }
    // storing word starting from location
    store_byte(location,0x000000FF & word);
    store_byte(location+1,(0x0000FF00 & word)>>8);
    store_byte(location+2,(0x00FF0000 & word)>>16);
    store_byte(location+3,(0xFF000000 & word)>>24);
    return 0;
}

void Memory::load_text_section(const std::string& machine_code_fname) {

    std::ifstream inFile;

    inFile.open(machine_code_fname, std::ios::in | std::ios::binary | std::ios::ate);

    if(inFile.is_open())
    {
        std::streamsize fsize = inFile.tellg();

        eof_text_section = (uint32_t)fsize - 1 ;

        inFile.seekg (0, std::ifstream::beg);

        if(!inFile.read((char *)memory_array,fsize)) {
            ::printf("Cannot read from input file\n");
            exit(0);
        }
        }else{
            ::printf("Cannot access input file\n");
            exit(0);
        }
}

void Memory::load_data_section(const std::string& data_section_fname) {

    std::ifstream inFile;

    inFile.open(data_section_fname, std::ios::in | std::ios::binary | std::ios::ate);

    if(inFile.is_open())
    {
        std::streamsize fsize = inFile.tellg();

        inFile.seekg (0, std::ifstream::beg);

        if(!inFile.read((char *)(memory_array+65536),fsize)) {
            ::printf("Cannot read from input file\n");
            exit(0);
        }
    }else{
        ::printf("Cannot access input file\n");
        exit(0);
    }
}   


uint32_t Memory::get_eot() const {
    return eof_text_section;
}
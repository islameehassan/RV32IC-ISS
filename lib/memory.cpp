/*
* Handles write and read to the simulated memory
*/

#include "memory.hpp"
#include "cstring"
#include <stdexcept>


Memory::Memory() {
    // Initialize memory with 0x0
    memset(memory_array,0,81920);
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
    return (256U*memory_array[location])+memory_array[location+1];
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
    return (16777216U*memory_array[location])+(65536U*memory_array[location+1])
           +(256U*memory_array[location+2])+memory_array[location+3];
}

int Memory::store_byte(int location, uint8_t byte) {
    // undefined access to memory
    try{
        if(location < 0x00000000 || location >= 0x00014000)
            throw std::out_of_range("memory accessing out of range");
    } catch (const std::out_of_range& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    // store byte in location
    memory_array[location] = byte;
    return 0;
}

int Memory::store_half_word(int location, uint16_t half_word) {
    // undefined access to memory
    try{
        if(location>=0x00013FFF)
            throw std::overflow_error("memory overflow");
    } catch (const std::overflow_error &o) {
        std::cout << "Exception caught: " << o.what() << std::endl;
    }
    // store half word starting from location
    store_byte(location, 0x00FF & half_word);
    store_byte(location+1, 0xFF00 & half_word);
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
    store_byte(location+1,0x0000FF00 & word);
    store_byte(location+2,0x00FF0000 & word);
    store_byte(location+3,0xFF000000 & word);
    return 0;
}

int Memory::load_text_section(std::string) {
    // read instructions from file and put them into memory text section;
    return 0;
}




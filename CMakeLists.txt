cmake_minimum_required(VERSION 3.22)
project(RV32IC_ISS)

set(CMAKE_CXX_STANDARD 14)

add_executable(RV32IC_ISS
        include/disassembler.hpp
        include/memory.hpp
        include/registerfile.hpp
        lib/disassembler.cpp
        lib/memory.cpp
        lib/registerfile.cpp
        main.cpp)

target_include_directories(RV32IC_ISS PUBLIC include/)
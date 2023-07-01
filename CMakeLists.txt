cmake_minimum_required(VERSION 3.24)
project(RV32IC_ISS)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(RV32IC_ISS
        include/disassembler.h
        include/memory.hpp
        include/registerfile.hpp
        include/riss.hpp
        lib/disassembler.cpp
        lib/memory.cpp
        lib/registerfile.cpp
        lib/riss.cpp
        main.cpp)

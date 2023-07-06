#include "disassembler.hpp"

int main(int argc, char *argv[]){

    if(argc == 1){
        std::cout << "use: rvsim <machine_code_file_name>\n";
        return 0;
    }

    std::string data_section_fname;
    std::string machine_code_fname = argv[1];
    std::cout << "Machine code file: " << machine_code_fname << std::endl;

    if(argc == 3){
        data_section_fname = argv[2];
    }

    disassembler dis(machine_code_fname,data_section_fname);

    dis.run();
}
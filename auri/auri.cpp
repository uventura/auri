#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

#include "auri/cli.hpp"

int main(int argc, char** argv) {
  Auri::Cli cli(argc, argv);
    std::cout<<cli.num_files()<<"\n";
  for (uint32_t i = 0; i < cli.num_files(); ++i) {
        std::ifstream file;
        file.open(cli.file(i));
        if(!file.is_open()){
            throw std::invalid_argument("The file '" + cli.file(i) + "' can't be found.");
        }
        
        char file_char;
        while(file.get(file_char)){
            std::cout<<file_char;
        }
  }
}

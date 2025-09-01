#include "auri/cli.hpp"
#include "auri/scanner.hpp"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    Auri::Cli cli(argc, argv);

    for (uint32_t i = 0; i < cli.num_files(); ++i) {
        Auri::Scanner scanner(cli.file(i));

        std::cout << "*------------------------------------------*" << std::endl;
        std::cout << "|                  Scanner                 |" << std::endl;
        std::cout << "*------------------------------------------*" << std::endl;
        for(auto i: scanner.tokens()) {
            std::cout << "|  Line " << i.line() << ": [" << i.typeToString() << "] .... Lexeme: " << i.lexeme() << " .... Literal: " << i.literal() << "\n";
        }
        std::cout << "*------------------------------------------*" << std::endl;
        std::cout << "*------------------------------------------*" << std::endl;
    }
}

#include "auri/ast_debug.hpp"
#include "auri/cli.hpp"
#include "auri/parser.hpp"
#include "auri/scanner.hpp"
#include "auri/token.hpp"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    Auri::Cli cli(argc, argv);

    for (uint32_t i = 0; i < cli.num_files(); ++i) {
        Auri::Scanner scanner(cli.file(i));
        scanner.debugPrint();

        Auri::AST::Parser parser(scanner.tokens());
        Auri::AST::AstDebug debug(parser.ast());
        debug.print();
    }
}

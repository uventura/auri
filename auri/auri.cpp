#include "auri/ast_debug.hpp"
#include "auri/cli.hpp"
#include "auri/interpreter.hpp"
#include "auri/parser.hpp"
#include "auri/scanner.hpp"

int main(int argc, char** argv) {
    Auri::Cli cli(argc, argv);

    for (uint32_t i = 0; i < cli.num_files(); ++i) {
        Auri::Scanner scanner(cli.file(i));
        Auri::AST::Parser parser(scanner.tokens());

        if (cli.enableTokenPrint()) {
            scanner.debugPrint();
        }
        if (cli.enableAstPrint()) {
            Auri::AST::AstDebug debug(parser.ast());
            debug.print();
        }

        Auri::AST::Interpreter interpreter(parser.runStatement());
        interpreter.run();
    }
}

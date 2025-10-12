#include "auri/core/cli.h"
#include "auri/core/scanner.h"
#include "auri/core/parser.h"
#include "auri/core/printer.h"
#include "auri/utils/string.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
    AuriCli cli = auri_cli(argc, argv);

    for(uint32_t i = 0; i < cli.file_paths.size; ++i) {
        AuriString* path = cli.file_paths.array[i];

        printf("Running '%s'...\n", path->text);
        AuriScanner scanner = auri_scanner(auri_strget(path));
        if(cli.enable_tokens){
            auri_print_tokens(&scanner);
        }

        AuriAst* ast = auri_parser(&scanner);
        if(cli.enable_ast) {
            auri_ast_print(ast);
        }

        auri_parser_free(ast);
        auri_scanner_free(&scanner);
        printf("End of '%s'\n\n", path->text);
    }
    auri_cli_free(&cli);
}

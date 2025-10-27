#include "auri/core/cli.h"
#include "auri/parser/scanner.h"
#include "auri/parser/parser.h"
#include "auri/parser/printer.h"
#include "auri/interpreter/interpreter.h"
#include "auri/utils/string.h"

#include <stdio.h>

void auri_interpreter(int argc, char* argv[]) {
    AuriCli cli = auri_cli(argc, argv);

    for(uint32_t i = 0; i < cli.file_paths.size; ++i) {
        AuriString* path = cli.file_paths.array[i];

        printf("[RUN ] .... '%s'\n", path->text);
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
        printf("[DONE] .... '%s'\n", path->text);
    }
    auri_cli_free(&cli);
}

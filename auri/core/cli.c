#include "auri/core/cli.h"
#include "auri/utils/basic_types.h"

#include <string.h>
#include <stdio.h>

AuriCli auri_cli(int argc, char* argv[]) {
    AuriCli cli;
    cli.help = false;
    cli.enable_ast = false;
    cli.enable_tokens = false;

    init_dynamic_ptr_array(&cli.file_paths, STRING_TYPE);

    for(int i=1; i < argc; ++i) {
        if(strcmp(argv[i], "--help") == 0) {
            if(i > 1)
                auri_throw_execution_error("The 'help' usage doesn't have extra elements!\n");

            auri_help();
            break;
        } else if(strcmp(argv[i], "--ast_debug") == 0) {
            cli.enable_ast = true;
        } else if(strcmp(argv[i], "--tokens_debug") == 0) {
            cli.enable_tokens = true;
        } else {
            void* path = argv[i];
            insert_dynamic_ptr_array(&cli.file_paths, path, strlen(path));
        }
    }

    return cli;
}

void auri_help() {
    printf("========================\n");
    printf("|         HELP         |\n");
    printf("========================\n");

    printf("<TODO>\n");

    printf("========================\n");
}

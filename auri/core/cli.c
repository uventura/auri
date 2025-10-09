#include "auri/core/cli.h"
#include "auri/utils/string.h"

#include <string.h>
#include <stdio.h>
#include <malloc.h>

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
        } else if(strcmp(argv[i], "--ast") == 0) {
            cli.enable_ast = true;
        } else if(strcmp(argv[i], "--tokens") == 0) {
            cli.enable_tokens = true;
        } else {
            AuriString* path = (AuriString*) malloc(sizeof(AuriString));
            auri_strinit(path);
            auri_strcat(path, argv[i], strlen(argv[i]));
            insert_dynamic_ptr_array(&cli.file_paths, path);
        }
    }

    return cli;
}

void auri_cli_free(AuriCli* cli) {
    free_dynamic_ptr_array(&cli->file_paths);
}

void auri_help() {
    printf("====================================\n");
    printf("|               HELP               |\n");
    printf("====================================\n");

    printf("| --tokens: Enable tokens visualization.\n");
    printf("| --ast: Enable AST visualization.\n");

    printf("====================================\n");
}

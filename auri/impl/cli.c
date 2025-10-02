#include "auri/impl/cli.h"

#include <string.h>
#include <stdio.h>

AuriCli auri_cli(int argc, char* argv[]) {
    AuriCli cli;
    cli.help = false;
    cli.enable_ast = false;
    cli.enable_tokens = false;

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
            cli.files_count++;
            strcpy(cli.file_paths[i-1], argv[i]);
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

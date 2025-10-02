#include "auri/cli.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
    AuriCli cli = auri_cli(argc, argv);
    for(uint32_t i = 0; i < cli.files_count; ++i) {
        printf(">> %s\n", cli.file_paths[i]);
    }
}
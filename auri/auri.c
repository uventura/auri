#include "auri/core/cli.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
    AuriCli cli = auri_cli(argc, argv);
    for(uint32_t i = 0; i < cli.file_paths.count; ++i) {
        printf(">> %s\n", (char*)cli.file_paths.array[i]);
    }

    printf("%d %d\n", cli.file_paths.count, cli.file_paths.size);
}
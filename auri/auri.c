#include "auri/core/cli.h"
#include "auri/utils/string.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
    AuriCli cli = auri_cli(argc, argv);
    for(uint32_t i = 0; i < cli.file_paths.size; ++i) {
        printf("%s\n", auri_strget(cli.file_paths.array[i]));
    }
    auri_cli_free(&cli);
}

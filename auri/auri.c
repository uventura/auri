#include "auri/core/cli.h"
#include "auri/core/scanner.h"
#include "auri/utils/string.h"
#include "auri/utils/debug.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
    auri_debug_print("Starting...\n");
    AuriCli cli = auri_cli(argc, argv);
    for(uint32_t i = 0; i < cli.file_paths.size; ++i) {
        AuriString* path = cli.file_paths.array[i];
        auri_scan(path->text);
    }
    auri_cli_free(&cli);
}

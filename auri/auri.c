#include "auri/core/cli.h"
#include "auri/core/scanner.h"
#include "auri/utils/string.h"
#include "auri/utils/debug.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
    AuriCli cli = auri_cli(argc, argv);
    for(uint32_t i = 0; i < cli.file_paths.size; ++i) {
        AuriString* path = cli.file_paths.array[i];

        AuriScanner scanner = auri_scanner(auri_strget(path));

        if(cli.enable_tokens){
            auri_print_tokens(&scanner);
        }

        auri_scanner_free(&scanner);
    }
    auri_cli_free(&cli);
}

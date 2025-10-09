// #include "auri/core/cli.h"

#include <stdio.h>
// #include <inttypes.h>

#include "auri/utils/print.h"

int main(int argc, char* argv[]) {
    // AuriCli cli = auri_cli(argc, argv);
    // for(uint32_t i = 0; i < cli.file_paths.size; ++i) {
    //     // printf("%s\n", (char*)cli.file_paths.array[i]);
    // }

    // printf("%d %d\n", cli.file_paths.size, cli.file_paths.size);
    // auri_print_char32(U'😀');
    auri_print("The emoticon is %s\n", U"--😀--   ");
}
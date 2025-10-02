#ifndef AURI_CLI_H
#define AURI_CLI_H

#include "auri/impl/common.h"
#include "auri/impl/environment.h"

typedef struct {
    char file_paths[MAX_PROVIDED_FILES][MAX_PATH_LENGTH];
    uint32_t files_count;
    bool help;
    bool enable_tokens;
    bool enable_ast;
} AuriCli;

AuriCli auri_cli(int argc, char* argv[]);
void auri_help();

#endif
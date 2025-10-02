#ifndef AURI_CLI_H
#define AURI_CLI_H

#include "auri/core/common.h"
#include "auri/core/environment.h"
#include "auri/utils/array.h"

typedef struct {
    DArrayVoidPtr file_paths;
    bool help;
    bool enable_tokens;
    bool enable_ast;
} AuriCli;

AuriCli auri_cli(int argc, char* argv[]);
void auri_help();

#endif
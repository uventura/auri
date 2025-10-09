#ifndef AURI_SCANNER_H
#define AURI_SCANNER_H

#include "auri/core/token.h"
#include "auri/utils/array.h"

#include <stdbool.h>

typedef struct {
    char* path;
    DArrayVoidPtr tokens;
} AuriScanner;

AuriScanner auri_scanner(const char* path);
void auri_print_tokens(AuriScanner* scanner);
void auri_scanner_free(AuriScanner* scanner);

#endif

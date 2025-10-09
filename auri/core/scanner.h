#ifndef AURI_SCANNER_H
#define AURI_SCANNER_H

#include "auri/core/tokens.h"
#include "auri/utils/array.h"

#include <stdbool.h>

typedef struct {
    char* path;
    DArrayVoidPtr tokens;
} AuriScanner;

AuriScanner auri_scan(const char* path);

#endif

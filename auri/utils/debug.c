#include "auri/utils/debug.h"

#include <stdio.h>

void _auri_print_file_info(const char* file, int line) {
    #ifdef ENABLE_DEBUG
    printf("[DEBUG] File: %s:%d | ", file, line);
    #endif
}

void _auri_debug_print(const char* message, ...) {
    #ifdef ENABLE_DEBUG
    va_list args;
    va_start(args, message);
    fprintf(stdout, message, args);
    va_end(args);
    #endif
}

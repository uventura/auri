#include "auri/core/common.h"
#include "auri/core/environment.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void auri_throw_execution_error(const char* error_message, ...) {
    va_list args;
    va_start(args, error_message);
    fprintf(stderr, error_message, args);
    va_end(args);
    exit(EXIT_FAILURE);
}
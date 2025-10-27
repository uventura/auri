#include "auri/core/exceptions.h"
#include "auri/core/environment.h"

#include <stdio.h>
#include <stdlib.h>

void auri_throw_execution_error(const char* error_message, ...) {
    va_list args;
    va_start(args, error_message);
    vprintf(error_message, args);
    va_end(args);
    exit(EXIT_FAILURE);
}

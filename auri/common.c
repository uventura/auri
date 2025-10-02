#include "auri/common.h"
#include "environment.h"

#include <stdio.h>
#include <stdlib.h>

void auri_throw_execution_error(const char* error_message) {
    fprintf(stderr, "%s", error_message);
    exit(EXIT_FAILURE);
}
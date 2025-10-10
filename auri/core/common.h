#ifndef AURI_COMMON_H
#define AURI_COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

void auri_throw_execution_error(const char* error_message, ...);

#endif

#ifndef AURI_DEBUG
#define AURI_DEBUG

#include <stdarg.h>

void _auri_print_file_info(const char* file, int line);
void _auri_debug_print(const char* message, ...);

#define auri_debug_print(message) \
    _auri_print_file_info(__FILE__, __LINE__); \
    _auri_debug_print(message)

#define auri_debug_fprint(...) \
    _auri_print_file_info(__FILE__, __LINE__); \
    _auri_debug_print(__VA_ARGS__)

#endif

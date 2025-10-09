#ifndef AURI_PRINT_H
#define AURI_PRINT_H

#include <uchar.h>

void auri_print_char32(char32_t ch);
void auri_print_char32_str(const char32_t *str);
void auri_print(const char* format, ...);

#endif
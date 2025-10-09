#include "auri/utils/string.h"
#include "auri/utils/print.h"
#include "auri/core/common.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <uchar.h>
#include <locale.h>
#include <inttypes.h>

void auri_print_char32(char32_t ch) {
    char utf8[5] = {0};
    int len = auri_char32_to_utf8(utf8, ch);
    if (len > 0) {
        printf("%s", utf8);
    } else {
        auri_throw_execution_error("Invalid char32_t: U+%" PRIXLEAST32 "\n", (uint_least32_t)ch);
    }
}

void auri_print_char32_str(const char32_t *str) {
    char utf8[5] = {0};
    while (*str) {
        int len = auri_char32_to_utf8(utf8, *str++);
        if (len > 0) {
            utf8[len] = '\0';
            fputs(utf8, stdout);
        } else {
            auri_throw_execution_error("Invalid string usage");
            fputs("[invalid]", stdout);
        }
    }
}

void auri_print(const char* format, ...) {
    va_list args;
    va_start(args, format);

    setlocale(LC_ALL, "");

    int32_t size = strlen(format);
    int32_t count = 0;

    while(*format) {
        count++;

        if(*format == '%' && count + 1 < size && *(format + 1) == 'c') {
            char32_t ch = va_arg(args, char32_t);
            auri_print_char32(ch);
            format++;
        }
        else if(*format == '%' && count + 1 < size && *(format + 1) == 's') {
            char32_t* str = va_arg(args, char32_t*);
            auri_print_char32_str(str);
            format++;
        }
        else {
            printf("%c", *format);
        }
        format++;
    }

    // fprintf(stderr, message, args);
    va_end(args);
}
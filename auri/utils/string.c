#include "auri/utils/string.h"
#include "auri/core/common.h"

#include <malloc.h>
#include <string.h>

uint32_t auri_text_size(char32_t* str) {
    char32_t* ptr = str;
    while (*ptr) {
        ++ptr;
    }
    return ptr - str;
}

void auri_strinit(AuriString* str) {
    str->text = (char32_t*) malloc(sizeof(char32_t) * AURI_STRING_START_SIZE);
    str->size = 0;
    str->capacity = AURI_STRING_START_SIZE;
}

int auri_char32_to_utf8(char *out, char32_t ch) {
    if (ch <= 0x7F) {
        out[0] = ch;
        return 1;
    } else if (ch <= 0x7FF) {
        out[0] = 0xC0 | ((ch >> 6) & 0x1F);
        out[1] = 0x80 | (ch & 0x3F);
        return 2;
    } else if (ch <= 0xFFFF) {
        out[0] = 0xE0 | ((ch >> 12) & 0x0F);
        out[1] = 0x80 | ((ch >> 6) & 0x3F);
        out[2] = 0x80 | (ch & 0x3F);
        return 3;
    } else if (ch <= 0x10FFFF) {
        out[0] = 0xF0 | ((ch >> 18) & 0x07);
        out[1] = 0x80 | ((ch >> 12) & 0x3F);
        out[2] = 0x80 | ((ch >> 6) & 0x3F);
        out[3] = 0x80 | (ch & 0x3F);
        return 4;
    }

    auri_throw_execution_error("Invalid char32 to utf8 conversion.\n");
    return 0;
}

char32_t auri_char_to_char32(char symbol) {
    char32_t out;
    mbstate_t ps = {0};
    size_t result = mbrtoc32(&out, &symbol, 1, &ps);
    if(result == (size_t)-1 || result == (size_t)-2) {
        auri_throw_execution_error("Error converting the symbol '%c'\n", symbol);
    }
    return out;
}

void auri_strcat_char32(AuriString* dest, char32_t* text) {
    uint32_t size = auri_text_size(text);

    if(dest->size + size > dest->capacity) {
        dest->capacity = (dest->size + size) * 2;
        dest->text = (char32_t*)realloc(&dest->text, dest->capacity * sizeof(char32_t));
    }

    for(uint32_t i = 0; i < size; ++i) {
        dest->text[dest->size + i] = text[i];
    }

    dest->size += size;
}

void auri_strcat_char(AuriString* dest, char* text) {
    uint32_t size = strlen(text);

    if(dest->size + size > dest->capacity) {
        dest->capacity = (dest->size + size) * 2;
        dest->text = (char32_t*)realloc(&dest->text, dest->capacity * sizeof(char32_t));
    }

    for(uint32_t i = 0; i < size; ++i) {
        char32_t symbol = auri_char_to_char32(text[i]);
        dest->text[dest->size + i] = symbol;
    }

    dest->size += size;
}

void auri_strfree(AuriString* str) {
    free(str->text);
    str->size = 0;
    str->capacity = 0;
}

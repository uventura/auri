#include "auri/utils/string.h"

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

char32_t auri_char_to_char32(char symbol) {
    char32_t out;
    mbstate_t ps = {0};
    mbrtoc32(&out, &symbol, 1, &ps);
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

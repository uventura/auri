#include "auri/utils/string.h"
#include "auri/core/common.h"

#include <malloc.h>
#include <string.h>
#include <stdio.h>

uint32_t auri_text_size(char* str) {
    char* ptr = str;
    while (*ptr) {
        ++ptr;
    }
    return ptr - str;
}

void auri_strinit(AuriString* str) {
    str->text = (char*) malloc(sizeof(char) * AURI_STRING_START_SIZE);
    str->size = 0;
    str->capacity = AURI_STRING_START_SIZE;
}

const char* auri_strget(AuriString* str) {
    return str->text;
}

void auri_strcat(AuriString* dest, char* text) {
    uint32_t size = strlen(text);

    if(dest->size + size > dest->capacity) {
        dest->capacity = (dest->size + size) * 2;
        dest->text = (char*)realloc(dest->text, dest->capacity * sizeof(char));
    }

    for(uint32_t i = 0; i < size; ++i) {
        dest->text[dest->size + i] = text[i];
    }

    dest->size += size;
}

void auri_strfree(AuriString* str) {
    free(str->text);
    str->text = NULL;
    str->size = 0;
    str->capacity = 0;
}

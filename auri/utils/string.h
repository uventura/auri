#ifndef AURI_STRING_H
#define AURI_STRING_H

#include "auri/utils/array.h"

#include <stdint.h>

#define AURI_STRING_START_SIZE 10

typedef struct {
    char* text;
    uint32_t size;
    uint32_t capacity;
} AuriString;

uint32_t auri_text_size(char* str);

void auri_strinit(AuriString* str);
const char* auri_strget(AuriString* str);
void auri_strcat(AuriString* dest, char* text);

void auri_strfree(AuriString* str);

#endif

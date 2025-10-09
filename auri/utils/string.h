#ifndef AURI_STRING_H
#define AURI_STRING_H

#include "auri/utils/array.h"

#include <stdint.h>

#define AURI_STRING_START_SIZE 1

typedef struct {
    char* text;
    uint32_t size;
    uint32_t capacity;
} AuriString;

uint32_t auri_text_size(char* str);

void auri_strinit(AuriString* str);
const char* auri_strget(AuriString* str);
char auri_strchar(AuriString* str, uint32_t pos);
void auri_strcat(AuriString* dest, char* text, uint32_t size);

void auri_strfree(AuriString* str);

#endif

#ifndef AURI_STRING_H
#define AURI_STRING_H

#include "auri/utils/array.h"

#include <stdint.h>
#include <uchar.h>

#define AURI_STRING_START_SIZE 10

typedef struct {
    char32_t* text;
    uint32_t size;
    uint32_t capacity;
} AuriString;

uint32_t auri_text_size(char32_t* str);

void auri_strinit(AuriString* str);

char32_t auri_char_to_char32(char symbol);
void auri_strcat_char(AuriString* dest, char* text);
void auri_strcat_char32(AuriString* dest, char32_t* text);

void auri_strfree(AuriString* str);

#endif

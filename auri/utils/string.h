#ifndef AURI_STRING_H
#define AURI_STRING_H

#include "auri/utils/array.h"

#include <stdint.h>
#include <wchar.h>

#define AURI_STRING_CHAR_TYPE char32_t
#define AURI_STRING_START_SIZE 10

typedef struct {
    AURI_STRING_CHAR_TYPE* text;
    uint32_t size;
    uint32_t _container_size;
} AuriString;

uint32_t auri_text_strsize(AURI_STRING_CHAR_TYPE* str);

void auri_strinit(AuriString* str);
void auri_strcat(AuriString* dest, AURI_STRING_CHAR_TYPE* text);
void auri_strfree(AuriString* str);

#endif

#include "auri/utils/string.h"

#include <malloc.h>

uint32_t auri_text_strsize(AURI_STRING_CHAR_TYPE* str) {
    AURI_STRING_CHAR_TYPE* ptr = str;
    while (*ptr) {
        ++ptr;
    }
    return ptr - str;
}

void auri_strinit(AuriString* str) {
    str->text = (AURI_STRING_CHAR_TYPE*) malloc(sizeof(AURI_STRING_CHAR_TYPE) * AURI_STRING_START_SIZE);
    str->size = 0;
    str->_container_size = AURI_STRING_START_SIZE;
}

void auri_strcat(AuriString* dest, AURI_STRING_CHAR_TYPE* text) {
    uint32_t size = auri_text_strsize(text);

    if(dest->size + size > dest->_container_size) {
        dest->_container_size = (dest->size + size) * 2;
        realloc(&dest->text, dest->_container_size * sizeof(AURI_STRING_CHAR_TYPE));
    }

    dest->size += size;
}

void auri_strfree(AuriString* str) {

}

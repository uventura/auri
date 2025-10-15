#ifndef AURI_ARRAY_UTIL
#define AURI_ARRAY_UTIL

#include <stdint.h>

typedef struct {
    void** array;
    uint32_t size;
    uint32_t capacity;
} DArrayVoidPtr;

void init_dynamic_ptr_array(DArrayVoidPtr* array);
void insert_dynamic_ptr_array(DArrayVoidPtr* array, void* element);
void free_dynamic_ptr_array(DArrayVoidPtr* array);

#endif

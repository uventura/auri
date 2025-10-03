#ifndef AURI_ARRAY_UTIL
#define AURI_ARRAY_UTIL

#include "auri/utils/basic_types.h"

#include <stdint.h>

typedef struct {
    void** array;
    uint32_t size;
    uint32_t count;
    BasicType type;
} DArrayVoidPtr;

void init_dynamic_ptr_array(DArrayVoidPtr* array, BasicType type);
void insert_dynamic_ptr_array(DArrayVoidPtr* array, void* element, uint32_t size);
void free_dynamic_ptr_array(DArrayVoidPtr* array);

#endif

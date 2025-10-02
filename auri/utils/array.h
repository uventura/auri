#ifndef AURI_ARRAY_UTIL
#define AURI_ARRAY_UTIL

#include "auri/utils/basic_types.h"

#include <stdint.h>

typedef struct {
    void** array;
    uint32_t size;
    uint32_t count;
    BasicType type;
} DynamicPtrArray;

void init_dynamic_ptr_array(DynamicPtrArray* array, BasicType type);
void insert_dynamic_ptr_array(DynamicPtrArray* array, void* element);
void free_dynamic_ptr_array(DynamicPtrArray* array);

#endif

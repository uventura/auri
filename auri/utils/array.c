#include "auri/utils/array.h"

#include <malloc.h>
#include <stddef.h>

void init_dynamic_ptr_array(DArrayVoidPtr* array, BasicType type) {
    array->size = 1;
    array->count = 0;
    array->array = NULL;
    array->type = type;
}

void insert_dynamic_ptr_array(DArrayVoidPtr* array, void* element, uint32_t size) {
    if(array->count + size >= array->size) {
        array->size = (array->size + size) * 2;
        array->array = realloc(array->array, array->size * sizeof(void*));
    }

    for(uint32_t i=0; i < size; ++i){
        array->array[array->count++] = *(element + i);
    }
}

void free_dynamic_ptr_array(DArrayVoidPtr* array) {
    free(array->array);
    array->array = NULL;
    array->size = 1;
    array->count = 0;
}

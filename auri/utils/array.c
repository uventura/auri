#include "auri/utils/array.h"

#include <malloc.h>
#include <stddef.h>

void init_dynamic_ptr_array(DynamicPtrArray* array, BasicType type) {
    array->size = 1;
    array->count = 0;
    array->array = NULL;
    array->type = type;
}

void insert_dynamic_ptr_array(DynamicPtrArray* array, void* element) {
    if(array->count + 1 >= array->size) {
        array->size *= 2;
        array->array = realloc(array->array, array->size * sizeof(void*));
    }

    array->array[array->count++] = element;
}

void free_dynamic_ptr_array(DynamicPtrArray* array) {
    free(array->array);
    array->array = NULL;
    array->size = 1;
    array->count = 0;
}

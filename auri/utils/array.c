#include "auri/utils/array.h"
#include "auri/utils/string.h"

#include <malloc.h>
#include <stddef.h>

void init_dynamic_ptr_array(DArrayVoidPtr* array, BasicType type) {
    array->size = 0;
    array->capacity = 1;
    array->array = NULL;
    array->type = type;
}

void insert_dynamic_ptr_array(DArrayVoidPtr* array, void* element) {
    if(array->size + 1 >= array->capacity) {
        array->capacity = (array->size + 1) * 2;
        array->array = realloc(array->array, array->size * sizeof(void*));
    }

    array->array[array->size] = element;
    array->size += 1;
}

void free_dynamic_ptr_array(DArrayVoidPtr* array) {
    for(uint32_t i=0; i < array->size; ++i) {
        if(array->type == STRING_TYPE) {
            auri_strfree(*array->array);
        } else {
            free(array->array[i]);
        }
    }

    array->array = NULL;
    array->size = 0;
    array->capacity = 0;
}

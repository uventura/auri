#include "auri/utils/array.h"
#include "auri/utils/string.h"
#include "auri/core/token.h"
#include "auri/core/ast_statement.h"

#include <stdio.h>
#include <stddef.h>
#include <malloc.h>

void init_dynamic_ptr_array(DArrayVoidPtr* array) {
    array->size = 0;
    array->capacity = 1;
    array->array = NULL;
}

void insert_dynamic_ptr_array(DArrayVoidPtr* array, void* element) {
    if(array->size + 1 >= array->capacity) {
        array->capacity = (array->capacity + array->size + 1) * 2;
        array->array = realloc(array->array, array->capacity * sizeof(void*));
    }

    array->array[array->size] = element;
    array->size += 1;
}

void free_dynamic_ptr_array(DArrayVoidPtr* array) {
    free(array->array);
    array->array = NULL;
    array->size = 0;
    array->capacity = 0;
}

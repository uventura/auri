#ifndef AURI_MEMORY_H
#define AURI_MEMORY_H

#include <malloc.h>

#define INCREASE_CAPACITY(capacity) capacity < 8? 8: capacity * 2
#define REALLOC_ARRAY(type, array, capacity) realloc(array, sizeof(type) * capacity)

#define AURI_INIT_DYN_ARRAY(capacity,size,array)\
    capacity = 0;\
    size = 0;\
    array = NULL

#define AURI_WRITE_DYN_ARRAY(type, array, value, size, capacity)\
    if(capacity < size + 1) {\
        capacity = INCREASE_CAPACITY(capacity);\
        array = REALLOC_ARRAY(type, array, capacity);\
    }\
    array[size] = value;\
    size++

#define AURI_FREE_DYN_ARRAY(array)\
    free(array);

#endif

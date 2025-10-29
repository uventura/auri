#ifndef AURI_CONSTANT_H
#define AURI_CONSTANT_H

#include "auri/core/common.h"

typedef double AuriConst;

typedef struct {
    AuriConst* array_value;
    uint32_t size;
    uint32_t capacity;
} AuriConstant;

void auri_constant_init(AuriConstant* constant);
void auri_constant_write(AuriConstant* constant, AuriConst value);
void auri_constant_free(AuriConstant* constant);

#endif

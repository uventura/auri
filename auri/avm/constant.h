#ifndef AURI_CONSTANT_H
#define AURI_CONSTANT_H

#include "auri/core/common.h"

typedef enum {
    CONST_DOUBLE_VAL,
    // CONST_INTEGER_VAL,
    CONST_BOOLEAN_VAL
} AuriVMValueType;

typedef union {
    double f64;
    bool boolean;
} AuriVMConstValue;

typedef struct {
    AuriVMValueType type;
    AuriVMConstValue value;
} AuriVMValue;

typedef struct {
    AuriVMValue* array_value;
    uint32_t size;
    uint32_t capacity;
} AuriConstant;

void auri_constant_init(AuriConstant* constant);
void auri_constant_write(AuriConstant* constant, AuriVMValue value);
void auri_constant_free(AuriConstant* constant);

#endif

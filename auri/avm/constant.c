#include "auri/avm/constant.h"

#include "auri/avm/memory.h"

void auri_constant_init(AuriConstant* constant) {
    AURI_INIT_DYN_ARRAY(constant->capacity,constant->size,constant->value);
}

void auri_constant_write(AuriConstant* constant, AuriConst value) {
    AURI_WRITE_DYN_ARRAY(
        AuriConst,
        constant->value,
        value,
        constant->size,
        constant->capacity
    );
}

void auri_constant_free(AuriConstant* constant) {
    AURI_FREE_DYN_ARRAY(constant->value);
    auri_constant_init(constant);
}

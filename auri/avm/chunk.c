#include "auri/avm/chunk.h"
#include "auri/core/common.h"
#include "auri/avm/constant.h"
#include "auri/avm/memory.h"

#include <malloc.h>

void auri_chunk_init(AuriChunk* chunk) {
    AURI_INIT_DYN_ARRAY(chunk->capacity, chunk->size, chunk->code);
    auri_constant_init(&chunk->constants);
}

void auri_chunk_write(AuriChunk* chunk, AuriOpCode code) {
    AURI_WRITE_DYN_ARRAY(AuriOpCode, chunk->code, code, chunk->size, chunk->capacity);
}

uint32_t auri_chunk_add_const(AuriChunk* chunk, AuriConst value) {
    auri_constant_write(&chunk->constants, value);
    return chunk->size - 1;
}

void auri_chunk_free(AuriChunk* chunk) {
    auri_constant_free(&chunk->constants);

    AURI_FREE_DYN_ARRAY(chunk->code);
    auri_chunk_init(chunk);
}

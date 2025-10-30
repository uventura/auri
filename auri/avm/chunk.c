#include "auri/avm/chunk.h"
#include "auri/core/common.h"
#include "auri/avm/constant.h"
#include "auri/avm/memory.h"

#include <malloc.h>

void auri_chunk_init(AuriChunk* chunk) {
    chunk->capacity = 0;
    chunk->size = 0;
    chunk->code = NULL;
    chunk->line = NULL;
    auri_constant_init(&chunk->constants);
}

void auri_chunk_write(AuriChunk* chunk, AuriOpCode code, uint32_t line) {
    if(chunk->capacity < chunk->size + 1) {
        chunk->capacity = INCREASE_CAPACITY(chunk->capacity);
        chunk->code = REALLOC_ARRAY(AuriOpCode, chunk->code, chunk->capacity);
        chunk->line = REALLOC_ARRAY(uint32_t, chunk->line, chunk->capacity);
    }
    chunk->code[chunk->size] = code;
    chunk->line[chunk->size] = line;
    chunk->size++;
}

uint32_t auri_chunk_add_const(AuriChunk* chunk, AuriVmValue value) {
    auri_constant_write(&chunk->constants, value);
    return chunk->constants.size - 1;
}

void auri_chunk_free(AuriChunk* chunk) {
    auri_constant_free(&chunk->constants);

    AURI_FREE_DYN_ARRAY(chunk->code);
    AURI_FREE_DYN_ARRAY(chunk->line);
    auri_chunk_init(chunk);
}

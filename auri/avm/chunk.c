#include "auri/avm/chunk.h"
#include "auri/core/common.h"
#include "auri/avm/memory.h"

#include <malloc.h>

void auri_chunk_init(AuriChunk* chunk) {
    AURI_INIT_DYN_ARRAY(chunk->capacity, chunk->size, chunk->code);
}

void auri_chunk_write(AuriChunk* chunk, AuriOpCode code) {
    AURI_WRITE_DYN_ARRAY(AuriOpCode, chunk->code, code, chunk->size, chunk->capacity);
}

void auri_chunk_free(AuriChunk* chunk) {
    AURI_FREE_DYN_ARRAY(chunk->code);
    auri_chunk_init(chunk);
}

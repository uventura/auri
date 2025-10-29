#ifndef AURI_CHUNK
#define AURI_CHUNK

#include "auri/core/common.h"
#include "auri/avm/constant.h"

typedef enum {
    OP_RETURN,
    OP_CONSTANT,
    OP_SUM,
    OP_SUB,
    OP_DIV,
    OP_MULT,
    OP_MOD,
} AuriOpCodeValue;

typedef uint8_t AuriOpCode;

typedef struct {
    AuriOpCode* code;
    uint32_t* line;
    AuriConstant constants;
    uint32_t size;
    uint32_t capacity;
} AuriChunk;

void auri_chunk_init(AuriChunk* chunk);
void auri_chunk_write(AuriChunk* chunk, AuriOpCode code, uint32_t line);
uint32_t auri_chunk_add_const(AuriChunk* chunk, AuriConst value);
void auri_chunk_free(AuriChunk* chunk);

#endif

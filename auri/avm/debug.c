#include "auri/avm/debug.h"
#include "auri/core/common.h"

#include <stdio.h>

uint32_t print_simple_instruction(const char* instruction, uint32_t offset);
uint32_t print_constant_instruction(AuriChunk* chunk, uint32_t offset);
uint32_t print_instruction(AuriChunk* chunk, uint32_t offset);

void auri_print_chunk_instructions(AuriChunk* chunk, const char* name) {
    printf("=== %s ===\n", name);

    for(uint32_t offset = 0; offset < chunk->size;) {
        offset = print_instruction(chunk, offset);
    }
}

uint32_t print_instruction(AuriChunk* chunk, uint32_t offset) {
    printf("%04d ", offset);

    switch (chunk->code[offset])
    {
        case OP_RETURN:
            return print_simple_instruction("<Return>", offset);
        case OP_CONSTANT:
            return print_constant_instruction(chunk, offset);
        case OP_SUM:
        case OP_SUB:
        case OP_DIV:
        case OP_MULT:
        case OP_MOD:
        default:
            printf("<Undefined OpCode>\n");
            return offset + 1;
    }
}

uint32_t print_simple_instruction(const char* instruction, uint32_t offset) {
    printf("%s\n", instruction);
    return offset + 1;
}

uint32_t print_constant_instruction(AuriChunk* chunk, uint32_t offset) {
    uint32_t index = chunk->code[offset + 1];
    printf("Constant: %lf\n", chunk->constants.value[index]);
    return offset + 2;
}

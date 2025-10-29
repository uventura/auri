#include "auri/avm/debug.h"
#include "auri/core/common.h"

#include <stdio.h>

uint32_t print_simple_instruction(const char* instruction, uint32_t offset);
uint32_t print_constant_instruction(const char* name, AuriChunk* chunk, uint32_t offset);

void auri_print_chunk_instructions(AuriChunk* chunk, const char* name) {
    printf("=== %s ===\n", name);

    for(uint32_t offset = 0; offset < chunk->size;) {
        offset = auri_print_chunk_instruction(chunk, offset);
    }

    printf("========================\n");
}

uint32_t auri_print_chunk_instruction(AuriChunk* chunk, uint32_t offset) {
    printf("[Line %d] %04d ", chunk->line[offset], offset);

    switch (chunk->code[offset])
    {
        case OP_RETURN:
            return print_simple_instruction("<Return>", offset);
        case OP_CONSTANT:
            return print_constant_instruction("<Constant>", chunk, offset);
        case OP_SUM:
            return print_simple_instruction("<Sum>", offset);
        case OP_SUB:
            return print_simple_instruction("<Subtraction>", offset);
        case OP_DIV:
            return print_simple_instruction("<Division>", offset);
        case OP_MULT:
            return print_simple_instruction("<Multiplication>", offset);
        case OP_MOD:
            return print_simple_instruction("<Mod>", offset);
        default:
            printf("<Undefined OpCode>\n");
            return offset + 1;
    }
}

uint32_t print_simple_instruction(const char* instruction, uint32_t offset) {
    printf("%s\n", instruction);
    return offset + 1;
}

uint32_t print_constant_instruction(const char* name, AuriChunk* chunk, uint32_t offset) {
    uint32_t index = chunk->code[offset + 1];
    printf("%s %g\n", name, chunk->constants.array_value[index]);
    return offset + 2;
}

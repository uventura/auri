#ifndef AURI_AVM_DEBUG
#define AURI_AVM_DEBUG

#include "auri/avm/chunk.h"

void auri_print_chunk_instructions(AuriChunk* chunk, const char* name);
uint32_t auri_print_chunk_instruction(AuriChunk* chunk, uint32_t offset);
const char* auri_print_instruction_name(AuriOpCodeValue opcode);

#endif

#ifndef AURI_AVM_H
#define AURI_AVM_H

#include "auri/avm/chunk.h"
#include "auri/avm/constant.h"

#define MAX_STACK_SIZE 1000
#define INIT_STACK_POSITION 0

typedef struct {
    AuriChunk* chunk;
    AuriOpCode* ip;
    AuriVMValue stack[MAX_STACK_SIZE];
    uint32_t stack_top;
} AuriVirtualMachine;

typedef enum {
    INTERPRET_OK,
    INTERPRET_RUNTIME_ERROR,
} AuriInterpretResult;

void auri_vm_init();
AuriInterpretResult auri_vm_interpret(AuriChunk* chunk);
void auri_vm_free();

#endif

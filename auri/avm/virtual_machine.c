#include "auri/avm/debug.h"
#include "auri/avm/virtual_machine.h"
#include "auri/core/exceptions.h"

#include <malloc.h>

AuriVirtualMachine* vm;
AuriInterpretResult vm_run();
AuriOpCode vm_instruction();

void vm_stack_debug();
void vm_stack_reset();
void vm_stack_push(AuriConst value);
AuriConst vm_stack_pop();

void auri_vm_init() {
    vm = (AuriVirtualMachine*) malloc(sizeof(AuriVirtualMachine));
    vm_stack_reset();
}

AuriInterpretResult auri_vm_interpret(AuriChunk* chunk) {
    vm->chunk = chunk;
    vm->ip = chunk->code;
    return vm_run();
}

void auri_vm_free() {
    // auri_chunk_free(vm->chunk);
    free(vm);
}

AuriInterpretResult vm_run() {
    for(;;) {
        #ifdef ENABLE_DEBUG
        vm_stack_debug();
        uint32_t offset = vm->ip - vm->chunk->code;
        auri_print_chunk_instruction(vm->chunk, offset);
        #endif

        AuriOpCode instruction = vm_instruction();

        switch(instruction) {
            case OP_CONSTANT: {
                AuriConst value = vm->chunk->constants.array_value[vm_instruction()];
                vm_stack_push(value);
                vm->ip++;
                break;
            }
            case OP_RETURN: {
                vm_stack_pop();
                return INTERPRET_OK;
            }
            default:
                return INTERPRET_RUNTIME_ERROR;
        }
    }
}

AuriOpCode vm_instruction() {
    AuriOpCode value = *vm->ip;
    vm->ip++;
    return value;
}

void vm_stack_debug() {
    printf("====== Stack ======\n");
    for(uint32_t i = 0; i < vm->stack_top; ++i) {
        printf("[ %g ]\n", vm->stack[i]);
    }
    printf("===== End Stack =====\n\n");
}

void vm_stack_reset() {
    vm->stack_top = INIT_STACK_POSITION;
}

void vm_stack_push(AuriConst value) {
    if(vm->stack_top + 1 >= MAX_STACK_SIZE) {
        auri_throw_execution_error("[Virtual Machine ERROR] Stack overflow.\n");
    }
    vm->stack[vm->stack_top++] = value;
}

AuriConst vm_stack_pop() {
    return vm->stack[vm->stack_top--];
}

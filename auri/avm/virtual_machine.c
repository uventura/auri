#include "auri/avm/debug.h"
#include "auri/avm/virtual_machine.h"
#include "auri/avm/debug.h"
#include "auri/core/exceptions.h"

#include <malloc.h>

AuriVirtualMachine* vm;
AuriInterpretResult vm_run();
AuriOpCode vm_instruction();

void vm_stack_debug();
void vm_stack_reset();
void vm_stack_push(AuriVMValue value);
AuriVMValue vm_stack_pop();
AuriVMValue vm_stack_peek();

void vm_stack_type_match_error(uint32_t size, ...);

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
    #define DOUBLE_BINARY_OPERATION(op)\
        do {\
        vm_stack_type_match_error(1, CONST_DOUBLE_VAL);\
        AuriVMValue a = vm_stack_pop();\
        vm_stack_type_match_error(1, CONST_DOUBLE_VAL);\
        AuriVMValue b = vm_stack_pop();\
        AuriVMValue r;\
        r.type = CONST_DOUBLE_VAL;\
        r.value.f64 = a.value.f64 op b.value.f64;\
        vm_stack_push(r);\
        } while(false)

    #define INTEGER_BINARY_OPERATION(op)\
        vm_stack_type_match_error(1, CONST_DOUBLE_VAL);\
        AuriVMValue a = vm_stack_pop();\
        vm_stack_type_match_error(1, CONST_DOUBLE_VAL);\
        AuriVMValue b = vm_stack_pop();\
        AuriVMValue r;\
        r.type = CONST_DOUBLE_VAL;\
        r.value.f64 = (uint32_t)a.value.f64 op (uint32_t)b.value.f64;\
        vm_stack_push(r)

    for(;;) {
        #ifdef ENABLE_DEBUG
        printf("\n.........................\n");
        vm_stack_debug();
        uint32_t offset = vm->ip - vm->chunk->code;
        auri_print_chunk_instruction(vm->chunk, offset);
        #endif

        AuriOpCode instruction = vm_instruction();

        switch(instruction) {
            case OP_CONSTANT: {
                AuriVMValue value = vm->chunk->constants.array_value[vm_instruction()];
                vm_stack_push(value);
                break;
            }
            case OP_NEGATIVE: {
                vm_stack_type_match_error(1, CONST_DOUBLE_VAL);
                AuriVMValue value = vm_stack_pop();
                value.value.f64 = -value.value.f64;
                vm_stack_push(value);
                break;
            }
            case OP_RETURN: {
                vm_stack_pop();
                return INTERPRET_OK;
            }
            case OP_SUM:
                DOUBLE_BINARY_OPERATION(+);
                break;
            case OP_SUB:
                DOUBLE_BINARY_OPERATION(-);
                break;
            case OP_DIV:
                DOUBLE_BINARY_OPERATION(/);
                break;
            case OP_MULT:
                DOUBLE_BINARY_OPERATION(*);
                break;
            case OP_MOD:
                INTEGER_BINARY_OPERATION(%);
                break;
            default:
                return INTERPRET_RUNTIME_ERROR;
        }

        #ifdef ENABLE_DEBUG
        printf(".........................\n");
        #endif
    }

    #undef DOUBLE_BINARY_OPERATION
    #undef INTEGER_BINARY_OPERATION
}

AuriOpCode vm_instruction() {
    AuriOpCode value = *vm->ip;
    vm->ip++;
    return value;
}

void vm_stack_debug() {
    printf("====== Stack ======\n");
    for(uint32_t i = 0; i < vm->stack_top; ++i) {
        printf("[");
        auri_print_const_value(vm->stack[i]);
        printf("]\n");
    }
    if(vm->stack_top == 0) {
        printf("<Empty Stack>\n");
    }
    printf("===== End Stack =====\n\n");
}

void vm_stack_reset() {
    vm->stack_top = INIT_STACK_POSITION;
}

void vm_stack_push(AuriVMValue value) {
    if(vm->stack_top + 1 >= MAX_STACK_SIZE) {
        auri_throw_execution_error("[Virtual Machine ERROR] Stack overflow.\n");
    }
    vm->stack[vm->stack_top++] = value;
}

AuriVMValue vm_stack_pop() {
    if(vm->stack_top <= 0) {
        auri_throw_execution_error("[StackError] the stack is empty.");
    }
    return vm->stack[--vm->stack_top];
}

AuriVMValue vm_stack_peek() {
    if(vm->stack_top <= 0) {
        auri_throw_execution_error("[StackError] the stack is empty.");
    }
    return vm->stack[vm->stack_top - 1];
}

void vm_stack_type_match_error(uint32_t size, ...) {
    va_list args;
    va_start(args, size);

    for(uint32_t i = 0; i < size; ++i) {
        AuriVMValueType type = va_arg(args, AuriVMValueType);
        if(type == vm_stack_peek().type) {
            return;
        }
    }
    va_end(args);

    auri_throw_execution_error("[StackTypeError] The stack doesn't match with the a correct type.\n");
    vm_stack_debug();

    return;
}

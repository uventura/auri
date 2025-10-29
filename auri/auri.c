#include "auri/core/cli.h"
#include "auri/interpreter/interpreter.h"
#include "auri/avm/chunk.h"
#include "auri/avm/debug.h"
#include "auri/avm/virtual_machine.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
    #ifdef RELEASE_MODE
    AuriCli cli = auri_cli(argc, argv);
    auri_interpreter(cli);
    auri_cli_free(&cli);
    #else
    AuriChunk chunk;
    auri_chunk_init(&chunk);

    uint32_t const_index = auri_chunk_add_const(&chunk, 42);
    auri_chunk_write(&chunk, OP_CONSTANT, 20);
    auri_chunk_write(&chunk, const_index, 20);

    auri_chunk_write(&chunk, OP_RETURN, 10);
    auri_chunk_write(&chunk, OP_RETURN, 15);

    auri_vm_init();
    auri_vm_interpret(&chunk);
    auri_vm_free();
    // auri_print_chunk_instructions(&chunk, "Print Chunks");

    auri_chunk_free(&chunk);
    #endif
}

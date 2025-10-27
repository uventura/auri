#include "auri/interpreter/interpreter.h"
#include "auri/avm/chunk.h"
#include "auri/avm/debug.h"

int main(int argc, char* argv[]) {
    AuriChunk chunk;
    auri_chunk_init(&chunk);

    auri_chunk_write(&chunk, OP_RETURN);
    auri_chunk_write(&chunk, OP_RETURN);

    auri_print_chunk_instructions(&chunk, "Print Chunks");

    auri_chunk_free(&chunk);
}

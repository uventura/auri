#include "auri/interpreter/interpreter.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
    printf("Running...\n");
    auri_interpreter(argc, argv);
}

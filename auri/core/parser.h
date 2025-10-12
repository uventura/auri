#ifndef AURI_PARSER_H
#define AURI_PARSER_H

#include "auri/core/scanner.h"
#include "auri/utils/array.h"

typedef struct {
    DArrayVoidPtr statements;
} AuriAst;

AuriAst* auri_parser(AuriScanner* scanner);
void auri_parser_free(AuriAst* ast);

#endif

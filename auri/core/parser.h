#ifndef AURI_PARSER_H
#define AURI_PARSER_H

#include "auri/core/scanner.h"

typedef enum {
    AST_NODE_LITERAL,
    AST_NODE_UNARY,
    AST_NODE_BINARY,
    AST_NODE_GROUPING
} AuriNodeType;

typedef struct Node {
    AuriNodeType type;
    AuriToken* token;
    struct Node* left;
    struct Node* right;
} AuriNode;

typedef struct {
    AuriNode* node;
} AuriAst;

AuriAst* auri_parser(AuriScanner* scanner);
void auri_parser_free(AuriAst* ast);

#endif

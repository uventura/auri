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
    AuriToken* token;
    AuriNodeType type;
    AuriToken* op;
    struct Node* left;
    struct Node* right;
} AuriNode;

typedef struct {
    AuriNode* node;
} AuriAst;

AuriAst* auri_parser(AuriScanner* scanner);

#endif

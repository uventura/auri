#ifndef AURI_AST_NODE_H
#define AURI_AST_NODE_H

#include "auri/parser/token.h"
#include "auri/utils/array.h"

#include <stdarg.h>

typedef enum {
    AST_NODE_LITERAL,
    AST_NODE_UNARY,
    AST_NODE_BINARY,
    AST_NODE_CALL,
    AST_NODE_GROUPING
} AuriNodeType;

typedef struct Node {
    AuriNodeType type;
    AuriToken* token;
    union {
        struct {int lit;} literal;
        struct {
            struct Node* expr;
        } unary;
        struct {
            struct Node* left;
            struct Node* right;
        } binary;
        struct {
            struct Node* callee;
            DArrayVoidPtr arguments;
        } call;
    };
} AuriNode;

AuriNode* ast_node_init(AuriNodeType type, AuriToken* token, ...);
void ast_node_free(AuriNode* node);

#endif

#ifndef AURI_AST_NODE_H
#define AURI_AST_NODE_H

#include "auri/core/token.h"

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

AuriNode* ast_node_init(AuriNodeType type, AuriToken* token, AuriNode* left, AuriNode* right);
void ast_node_free(AuriNode* node);

#endif

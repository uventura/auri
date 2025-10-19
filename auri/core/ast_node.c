#include "auri/core/ast_node.h"
#include "auri/core/token.h"

#include "auri/core/common.h"

#include <malloc.h>
#include <stddef.h>

AuriNode* literal_node_init(AuriToken* token);
AuriNode* unary_node_init(AuriToken* token, AuriNode* expr);
AuriNode* binary_node_init(AuriToken* token, AuriNode* left, AuriNode* right);

void node_unary_child_free(AuriNode* expr);
void node_binary_child_free(AuriNode* node);

AuriNode* ast_node_init(AuriNodeType type, AuriToken* token, ...) {
    va_list args;
    va_start(args, token);

    if(type == AST_NODE_BINARY) {
        AuriNode* left = va_arg(args, AuriNode*);
        AuriNode* right = va_arg(args, AuriNode*);
        va_end(args);
        return binary_node_init(token, left, right);
    }

    if(type == AST_NODE_UNARY) {
        AuriNode* expr = va_arg(args, AuriNode*);
        va_end(args);
        return unary_node_init(token, expr);
    }

    if(type == AST_NODE_LITERAL) {
        AuriNode* node = (AuriNode*) malloc(sizeof(AuriNode));
        node->type = AST_NODE_LITERAL;
        node->token = token;
        va_end(args);
        return node;
    }

    auri_throw_execution_error("Wrong ast node type on line %d.\n", token->line);
    return NULL;
}

AuriNode* literal_node_init(AuriToken* token) {
    AuriNode* node = (AuriNode*) malloc(sizeof(AuriNode));
    node->type = AST_NODE_LITERAL;
    node->token = token;

    return node;
}

AuriNode* unary_node_init(AuriToken* token, AuriNode* expr) {
    AuriNode* node = (AuriNode*) malloc(sizeof(AuriNode));
    node->type = AST_NODE_UNARY;
    node->token = token;
    node->unary.expr = expr;

    return node;
}

AuriNode* binary_node_init(AuriToken* token, AuriNode* left, AuriNode* right) {
    AuriNode* node = (AuriNode*) malloc(sizeof(AuriNode));
    node->type = AST_NODE_BINARY;
    node->token = token;
    node->binary.left = left;
    node->binary.right = right;

    return node;
}

void ast_node_free(AuriNode* node) {
    if(node == NULL) return;

    if(node->type == AST_NODE_BINARY) {
        node_binary_child_free(node);
    } else if(node->type == AST_NODE_UNARY) {
        node_unary_child_free(node);
    }

    free(node);
}

void node_unary_child_free(AuriNode* node) {
    if(node->unary.expr != NULL) {
        ast_node_free(node->unary.expr);
        free(node->unary.expr);

        node->unary.expr = NULL;
    }
}

void node_binary_child_free(AuriNode* node) {
    if(node->binary.left != NULL) {
        ast_node_free(node->binary.left);
        node->binary.left = NULL;
    }

    if(node->binary.right != NULL) {
        ast_node_free(node->binary.right);
        node->binary.right = NULL;
    }
}

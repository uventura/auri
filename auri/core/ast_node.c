#include "auri/core/ast_node.h"
#include "auri/core/token.h"

#include <malloc.h>
#include <stddef.h>

AuriNode* ast_node_init(AuriNodeType type, AuriToken* token, AuriNode* left, AuriNode* right) {
    AuriNode* node = (AuriNode*) malloc(sizeof(AuriNode));
    node->type = type;
    node->token = token;
    node->left = left;
    node->right = right;

    return node;
}

void ast_node_free(AuriNode* node) {
    if(node->left != NULL) {
        ast_node_free(node->left);
        free(node->left);
    }

    if(node->right != NULL) {
        ast_node_free(node->right);
        free(node->right);
    }
}

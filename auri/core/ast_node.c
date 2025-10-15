#include "auri/core/ast_node.h"
#include "auri/core/token.h"

#include <malloc.h>
#include <stddef.h>

void node_child_free(AuriNode* node);

AuriNode* ast_node_init(AuriNodeType type, AuriToken* token, AuriNode* left, AuriNode* right) {
    AuriNode* node = (AuriNode*) malloc(sizeof(AuriNode));
    node->type = type;
    node->token = token;
    node->left = left;
    node->right = right;

    return node;
}

void ast_node_free(AuriNode* node) {
    if(node == NULL) return;
    node_child_free(node);
    free(node);
}

void node_child_free(AuriNode* node) {
    if(node->left != NULL) {
        node_child_free(node->left);
        free(node->left);

        node->left = NULL;
    }

    if(node->right != NULL) {
        node_child_free(node->right);
        free(node->right);

        node->right = NULL;
    }
}

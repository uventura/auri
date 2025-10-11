#include "auri/core/printer.h"

#include <stdio.h>

void auri_ast_print_node(AuriNode* node);
void ast_print_literal(AuriNode* node);

void auri_ast_print(AuriNode* node) {
    auri_ast_print_node(node);
}

void auri_ast_print_node(AuriNode* node) {
    switch(node->type) {
        case AST_NODE_LITERAL:
            ast_print_literal(node);
            break;
        case AST_NODE_UNARY:
            break;
        case AST_NODE_BINARY:
            break;
        case AST_NODE_GROUPING:
            break;
    }
}

void ast_print_literal(AuriNode* node) {
    switch(node->token->type) {
        case AR_TOKEN_STRING:
            printf("%s", node->token->literal.string.text);
            break;
        case AR_TOKEN_NUMBER:
            printf("%lf", node->token->literal.numeric);
            break;
        case AR_TOKEN_TRUE:
            printf("true");
            break;
        case AR_TOKEN_FALSE:
            printf("false");
            break;
        default:
            break;
    }
    printf("\n");
}

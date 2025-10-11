#include "auri/core/printer.h"

#include <stdio.h>

void ast_print_node(AuriNode* node);
void ast_print_unary(AuriNode* node);
void ast_print_binary(AuriNode* node);
void ast_print_literal(AuriNode* node);

void auri_ast_print(AuriNode* node) {
    printf("=========================\n");
    printf("|           AST         |\n");
    printf("=========================\n");

    ast_print_node(node);

    printf("\n=========================\n");
}

void ast_print_node(AuriNode* node) {
    switch(node->type) {
        case AST_NODE_LITERAL:
            ast_print_literal(node);
            break;
        case AST_NODE_UNARY:
            ast_print_unary(node);
            break;
        case AST_NODE_BINARY:
            ast_print_binary(node);
            break;
        case AST_NODE_GROUPING:
            break;
    }
}

void ast_print_unary(AuriNode* node) {
    printf("%s", node->token->lexeme.text);
    ast_print_node(node->right);
}

void ast_print_binary(AuriNode* node) {
    ast_print_node(node->left);
    printf("Op: %s\n", node->token->lexeme.text);
    ast_print_node(node->right);
}

void ast_print_literal(AuriNode* node) {
    switch(node->token->type) {
        case AR_TOKEN_STRING:
            printf("%s", node->token->lexeme.text);
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

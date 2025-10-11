#include "auri/core/printer.h"

#include <stdio.h>
#include <stdint.h>

uint32_t ast_spaces_counter = 0;

void ast_print_spaces(void);

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

void ast_print_spaces(void) {
    for(uint32_t i = 0; i < ast_spaces_counter; ++i) printf("|    ");
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
    ast_print_spaces();
    printf("[Unary] <<<\n");

    ast_spaces_counter++;
    ast_print_spaces();
    printf("  \\\n");

    ast_print_spaces();
    printf(" [%s]\n", node->token->lexeme.text);

    ast_print_spaces();
    printf("  /\n");
    ast_spaces_counter--;

    ast_spaces_counter++;
    ast_print_node(node->right);
    ast_spaces_counter--;

    ast_print_spaces();
    printf(">>>\n");
}

void ast_print_binary(AuriNode* node) {
    ast_print_spaces();
    printf("[Binary] <<< \n");

    ast_spaces_counter++;
    ast_print_node(node->left);
    ast_spaces_counter--;

    ast_spaces_counter++;
    ast_print_spaces();
    printf("  \\\n");

    ast_print_spaces();
    printf("  [%s]\n", node->token->lexeme.text);

    ast_print_spaces();
    printf("  /\n");
    ast_spaces_counter--;

    ast_spaces_counter++;
    ast_print_node(node->right);
    ast_spaces_counter--;

    ast_print_spaces();
    printf(">>>\n");
}

void ast_print_literal(AuriNode* node) {
    ast_print_spaces();
    switch(node->token->type) {
        case AR_TOKEN_STRING:
            printf(" (%s)\n", node->token->lexeme.text);
            break;
        case AR_TOKEN_NUMBER:
            printf(" (%lf)\n", node->token->literal.numeric);
            break;
        case AR_TOKEN_TRUE:
            printf(" (true)\n");
            break;
        case AR_TOKEN_FALSE:
            printf(" (false)\n");
            break;
        default:
            break;
    }
}

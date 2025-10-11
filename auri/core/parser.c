#include "auri/core/parser.h"

#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

AuriScanner* auri_parser_scanner;
AuriNode* auri_parser_node;
uint32_t auri_parser_token_pos = 0;

AuriNode* node_init(AuriNodeType type, AuriToken* token, AuriNode* left, AuriNode* right);

AuriNode* expression(void);
AuriNode* factor(void);
AuriNode* unary(void);
AuriNode* primary(void);

AuriToken* parser_peek(void);
AuriToken* parser_previous(void);
AuriToken* parser_advance(void);
bool parser_match(uint32_t size, ...);
bool parser_is_at_end(AuriScanner* scanner);

AuriAst* auri_parser(AuriScanner* scanner) {
    AuriAst* ast = (AuriAst*)malloc(sizeof(AuriAst));
    auri_parser_scanner = scanner;
    auri_parser_token_pos = 0;

    while(parser_is_at_end(scanner)) {
        auri_parser_node = expression();
    }

    ast->node = auri_parser_node;
    return ast;
}

AuriNode* expression(void) {
    return factor();
}

AuriNode* factor(void) {
    AuriNode* node = unary();

    AuriNode* current = node;
    while(parser_match(2, AR_TOKEN_STAR, AR_TOKEN_SLASH)) {
        current->type = AST_NODE_BINARY;
        current->left = current;
        current->token = parser_previous();
        current->right = unary();

        current = current->right;
    }

    printf("Left: %lf\n", node->left->token->literal.numeric);
    printf("Operator: %s\n", node->token->lexeme.text);
    return node;
}
/*
1 * 23 * 56

   *
 /   \
1     *
    /   \
  23     56

*/
AuriNode* unary(void) {
    if(parser_match(2, AR_TOKEN_BANG_EQUAL, AR_TOKEN_MINUS)) {
        AuriToken* operator = parser_previous();
        return node_init(AST_NODE_UNARY, operator, NULL, unary());
    }

    return primary();
}

AuriNode* primary(void) {
    if(parser_match(5, AR_TOKEN_NUMBER, AR_TOKEN_STRING, AR_TOKEN_TRUE, AR_TOKEN_FALSE, AR_TOKEN_NULL)) {
        return node_init(AST_NODE_LITERAL, parser_previous(), NULL, NULL);
    }

    auri_throw_execution_error("Literal undefined (Line: %d)[%s]: %s\n",
        parser_peek()->line + 1, auri_token_name(parser_peek()->type), parser_peek()->lexeme.text);
    return NULL;
}

AuriNode* node_init(AuriNodeType type, AuriToken* token, AuriNode* left, AuriNode* right) {
    AuriNode* node = (AuriNode*) malloc(sizeof(AuriNode));
    node->type = type;
    node->token = token;
    node->left = left;
    node->right = right;

    return node;
}

bool parser_match(uint32_t size, ...) {
    va_list args;
    va_start(args, size);
    for(uint32_t i = 0; i < size; ++i) {
        AuriTokenType type = va_arg(args, AuriTokenType);
        if(parser_peek()->type == type) {
            parser_advance();
            return true;
        }
    }
    va_end(args);

    return false;
}

AuriToken* parser_peek(void) {
    return auri_parser_scanner->tokens.array[auri_parser_token_pos];
}

AuriToken* parser_previous(void) {
    if(auri_parser_token_pos - 1 < 0) {
        return auri_parser_scanner->tokens.array[auri_parser_token_pos];
    }
    return auri_parser_scanner->tokens.array[auri_parser_token_pos - 1];
}

AuriToken* parser_advance(void) {
    if(auri_parser_token_pos + 1 >= auri_parser_scanner->tokens.size) {
        return NULL;
    }

    return auri_parser_scanner->tokens.array[auri_parser_token_pos++];
}

bool parser_is_at_end(AuriScanner* scanner) {
    AuriToken* token = scanner->tokens.array[auri_parser_token_pos];

    return token->type != AR_TOKEN_EOF;
}

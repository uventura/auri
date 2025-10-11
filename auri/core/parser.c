#include "auri/core/parser.h"

#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

AuriScanner* auri_parser_scanner;
AuriNode* auri_parser_node;
uint32_t auri_parser_token_pos = 0;

AuriNode* node_init(AuriToken* token, AuriNodeType type, AuriToken* op, AuriNode* left, AuriNode* right);

AuriNode* expression(void);
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
    return primary();
}

AuriNode* primary(void) {
    if(parser_match(5, AR_TOKEN_NUMBER, AR_TOKEN_STRING, AR_TOKEN_TRUE, AR_TOKEN_FALSE, AR_TOKEN_NULL)) {
        return node_init(parser_previous(), AST_NODE_LITERAL, NULL, NULL, NULL);
    }

    auri_throw_execution_error("Token undefined");
    return NULL;
}

AuriNode* node_init(AuriToken* token, AuriNodeType type, AuriToken* op, AuriNode* left, AuriNode* right) {
    AuriNode* node = (AuriNode*) malloc(sizeof(AuriNode));
    node->token = token;
    node->type = type;
    node->op = op;
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

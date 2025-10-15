#include "auri/core/parser.h"
#include "auri/core/ast_node.h"
#include "auri/core/ast_statement.h"

#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

AuriScanner* auri_parser_scanner;
uint32_t auri_parser_token_pos = 0;

// Statements
AuriStmt* statement(void);
AuriStmt* import_stmt(void);
AuriStmt* expression_stmt(void);
AuriStmt* block_stmt(void);
AuriStmt* if_stmt(void);

// Expressions
AuriNode* expression(void);
AuriNode* assignment(void);
AuriNode* or(void);
AuriNode* and(void);
AuriNode* equality(void);
AuriNode* comparison(void);
AuriNode* term(void);
AuriNode* factor(void);
AuriNode* unary(void);
AuriNode* primary(void);

// Parser
AuriToken* parser_peek(void);
AuriToken* parser_previous(void);
AuriToken* parser_advance(void);

bool parser_match(uint32_t size, ...);
bool parser_is_at_end(AuriScanner* scanner);

//||========================================================||
//||========================================================||
//||========================================================||

AuriAst* auri_parser(AuriScanner* scanner) {
    AuriAst* ast = (AuriAst*)malloc(sizeof(AuriAst));
    init_dynamic_ptr_array(&ast->statements, STATEMENT_TYPE);

    auri_parser_scanner = scanner;
    auri_parser_token_pos = 0;

    while(!parser_is_at_end(scanner)) {
        AuriStmt* stmt = statement();
        insert_dynamic_ptr_array(&ast->statements, stmt);
    }

    return ast;
}

void auri_parser_free(AuriAst* ast) {
    for(uint32_t i = 0; i < ast->statements.size; ++i){
        printf("Free!!\n");
        AuriStmt* stmt = ast->statements.array[i];
        auri_stmt_free(stmt);
    }
    free_dynamic_ptr_array(&ast->statements);
    free(ast);
}

//||========================================================||
//||========================================================||
//||========================================================||

//+-------------+
//|  STATEMENTS |
//+-------------+

AuriStmt* statement(void) {
    if(parser_match(1, AR_TOKEN_IMPORT)) return import_stmt();
    else if(parser_match(1, AR_TOKEN_IF)) return if_stmt();
    else if(parser_match(1, AR_TOKEN_LEFT_BRACE)) return block_stmt();

    return expression_stmt();
}

AuriStmt* import_stmt(void) {
    return NULL;
}

AuriStmt* expression_stmt(void) {
    AuriNode* expr = expression();
    if(!parser_match(1, AR_TOKEN_SEMICOLON)) {
        auri_throw_execution_error("Missing ';' on line %d.\n", parser_peek()->line);
    }

    AuriStmtNode expr_stmt;
    expr_stmt.expr.item = expr;

    return auri_stmt_init(AST_STMT_EXPR, expr_stmt);
}

AuriStmt* block_stmt(void) {
    DArrayVoidPtr items;
    init_dynamic_ptr_array(&items, STATEMENT_TYPE);

    while(!parser_match(1, AR_TOKEN_RIGHT_BRACE)) {
        AuriStmt* stmt = statement();
        insert_dynamic_ptr_array(&items, stmt);

        if(parser_is_at_end(auri_parser_scanner)) {
            auri_throw_execution_error("Missing '}' on line %d.\n", parser_peek()->line);
        }
    }

    AuriStmtNode block_stmt;
    block_stmt.block.items = items;

    return auri_stmt_init(AST_STMT_BLOCK, block_stmt);
}

AuriStmt* if_stmt(void) {
    if(!parser_match(1, AR_TOKEN_LEFT_PAREN)) {
        auri_throw_execution_error("Missing '(' on if statement on line %d.\n", parser_peek()->line);
    }

    AuriNode* expr = expression();

    if(!parser_match(1, AR_TOKEN_RIGHT_PAREN)) {
        auri_throw_execution_error("Missing ')' on if statement on line %d.\n", parser_peek()->line);
    }
    AuriStmt* stmt = statement();

    AuriStmtNode if_stmt;
    if_stmt.if_else.expr = expr;
    if_stmt.if_else.block = stmt;

    return auri_stmt_init(AST_STMT_IF, if_stmt);
}

//+-------------+
//| EXPRESSIONS |
//+-------------+

#define BINARY_NODE(func, comparison_args_size, ...) \
    AuriNode* node = func();\
    AuriNode* current = node;\
    while(parser_match(comparison_args_size, __VA_ARGS__)) {\
        current->left = ast_node_init(current->type, current->token, current->left, current->right);\
        current->type = AST_NODE_BINARY;\
        current->token = parser_previous();\
        current->right = func();\
        current = current->right;\
    }\
    return node

AuriNode* expression(void) {
    return or();
}

AuriNode* assignment(void) {
    return NULL;
}

AuriNode* or(void) {
    AuriNode* node = and();
    AuriNode* current = node;

    while(parser_match(1, AR_TOKEN_OR)) {
        current->left = ast_node_init(current->type, current->token, current->left, current->right);
        current->type = AST_NODE_BINARY;
        current->token = parser_previous();
        current->right = equality();
        current = current->right;
    }

    return node;
}

AuriNode* and(void) {
    BINARY_NODE(equality, 1, AR_TOKEN_AND);
}

AuriNode* equality(void) {
    BINARY_NODE(comparison, 2, AR_TOKEN_EQUAL_EQUAL, AR_TOKEN_BANG_EQUAL);
}

AuriNode* comparison(void) {
    BINARY_NODE(term, 4, AR_TOKEN_GREATER, AR_TOKEN_GREATER_EQUAL, AR_TOKEN_LESS, AR_TOKEN_LESS_EQUAL);
}

AuriNode* term(void) {
    BINARY_NODE(factor, 2, AR_TOKEN_MINUS, AR_TOKEN_PLUS);
}

AuriNode* factor(void) {
    BINARY_NODE(unary, 2, AR_TOKEN_STAR, AR_TOKEN_SLASH);
}

AuriNode* unary(void) {
    if(parser_match(2, AR_TOKEN_BANG_EQUAL, AR_TOKEN_MINUS)) {
        AuriToken* operator = parser_previous();
        return ast_node_init(AST_NODE_UNARY, operator, NULL, unary());
    }

    return primary();
}

AuriNode* primary(void) {
    if(parser_match(5, AR_TOKEN_NUMBER, AR_TOKEN_STRING, AR_TOKEN_TRUE, AR_TOKEN_FALSE, AR_TOKEN_NULL)) {
        return ast_node_init(AST_NODE_LITERAL, parser_previous(), NULL, NULL);
    }

    if(parser_match(1, AR_TOKEN_LEFT_PAREN)) {
        AuriNode* expr = expression();
        if(!parser_match(1, AR_TOKEN_RIGHT_PAREN)) {
            auri_throw_execution_error("No matching parenthesis in %d\n!", parser_peek()->line);
        }

        return expr;
    }

    auri_throw_execution_error("Literal undefined (Line: %d)[%s]: %s\n",
        parser_peek()->line + 1, auri_token_name(parser_peek()->type), parser_peek()->lexeme.text);
    return NULL;
}

//+-------------+
//|    PARSER   |
//+-------------+

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

    return token->type == AR_TOKEN_EOF;
}

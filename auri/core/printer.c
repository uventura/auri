#include "auri/core/printer.h"
#include "auri/core/ast_node.h"
#include "auri/core/ast_statement.h"

#include <stdio.h>
#include <stdint.h>

uint32_t ast_spaces_counter = 0;

void ast_print_spaces(void);

void ast_print_stmt(AuriStmt* stmt);
void ast_print_expr_stmt(AuriStmt* stmt);
void ast_print_block_stmt(AuriStmt* stmt);
void ast_print_if_stmt(AuriStmt* stmt);

void ast_print_node(AuriNode* node);
void ast_print_unary(AuriNode* node);
void ast_print_binary(AuriNode* node);
void ast_print_literal(AuriNode* node);

void auri_ast_print(AuriAst* ast) {
    printf("=========================\n");
    printf("|           AST         |\n");
    printf("=========================\n");

    for(uint32_t i = 0; i < ast->statements.size; ++i) {
        ast_print_stmt(ast->statements.array[i]);
        printf("\n");
    }

    printf("\n=========================\n");
}

void ast_print_spaces(void) {
    for(uint32_t i = 0; i < ast_spaces_counter; ++i) printf("|    ");
}

void ast_print_stmt(AuriStmt* stmt) {
    if(stmt == NULL) return;

    switch (stmt->type)
    {
        case AST_STMT_EXPR:
            ast_print_expr_stmt(stmt);
            break;
        case AST_STMT_BLOCK:
            ast_print_block_stmt(stmt);
            break;
        case AST_STMT_RUN:
        case AST_STMT_IF:
            ast_print_if_stmt(stmt);
            break;
        case AST_STMT_WHILE:
        case AST_STMT_FOR:
        case AST_STMT_VAR:
        case AST_STMT_IMPORT:
        case AST_STMT_RETURN:
        case AST_STMT_BREAK:
        case AST_STMT_CONTINUE:
        default:
            break;
    }
}

void ast_print_expr_stmt(AuriStmt* stmt) {
    ast_print_spaces();
    printf("{ExpressionStmt} <<\n");

    ast_spaces_counter++;
    ast_print_node(stmt->stmt.expr.item);
    ast_spaces_counter--;

    ast_print_spaces();
    printf(">>\n");
}

void ast_print_block_stmt(AuriStmt* stmt) {
    ast_print_spaces();
    printf("{BlockStmt} << \n");

    for(uint32_t i = 0; i < stmt->stmt.block.items.size; ++i){
        ast_spaces_counter++;
        ast_print_stmt(stmt->stmt.block.items.array[i]);
        ast_spaces_counter--;
    }

    ast_print_spaces();
    printf(">>\n");
}

void ast_print_if_stmt(AuriStmt* stmt) {
    ast_print_spaces();
    printf("{IfStmt} << \n");

    ast_spaces_counter++;
    ast_print_node(stmt->stmt.if_else.expr);
    ast_spaces_counter--;

    ast_spaces_counter++;
    ast_print_stmt(stmt->stmt.if_else.if_block);
    ast_spaces_counter--;

    ast_print_spaces();
    printf(">>\n");

    ast_print_spaces();
    printf("{ElseStmt} << \n");

    ast_spaces_counter++;
    ast_print_stmt(stmt->stmt.if_else.else_block);
    ast_spaces_counter--;

    ast_print_spaces();
    printf(">>\n");
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

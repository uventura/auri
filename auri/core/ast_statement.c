#include "auri/core/ast_statement.h"
#include "auri/utils/array.h"
#include "auri/core/ast_node.h"

#include <inttypes.h>
#include <stdio.h>
#include <malloc.h>

void stmt_block_free(AuriStmt* stmt);
void stmt_if_block_free(AuriStmt* stmt);

AuriStmt* auri_stmt_init(AuriStmtType type, AuriStmtNode node) {
    AuriStmt* stmt = (AuriStmt*) malloc(sizeof(AuriStmt));
    stmt->type = type;
    stmt->stmt = node;
    return stmt;
}

void auri_stmt_free(AuriStmt* stmt) {
    if(stmt == NULL) return;

    switch(stmt->type) {
        case AST_STMT_EXPR:
            ast_node_free(stmt->stmt.expr.item);
            stmt->stmt.expr.item = NULL;
            break;
        case AST_STMT_BLOCK:
            stmt_block_free(stmt);
            break;
        case AST_STMT_RUN:
            auri_stmt_free(stmt->stmt.run.block);
            break;
        case AST_STMT_IF:
            stmt_if_block_free(stmt);
            break;
        case AST_STMT_WHILE:
        case AST_STMT_FOR:
        case AST_STMT_VAR:
        case AST_STMT_IMPORT:
        case AST_STMT_RETURN:
        case AST_STMT_BREAK:
        case AST_STMT_CONTINUE:
            break;
        default:
            break;
    }

    free(stmt);
}

void stmt_block_free(AuriStmt* stmt) {
    for(uint32_t i = 0; i < stmt->stmt.block.items.size; ++i) {
        AuriStmt* inner_stmt = stmt->stmt.block.items.array[i];
        auri_stmt_free(inner_stmt);
    }
    free_dynamic_ptr_array(&stmt->stmt.block.items);
}

void stmt_if_block_free(AuriStmt* stmt) {
    ast_node_free(stmt->stmt.if_else.expr);
    auri_stmt_free(stmt->stmt.if_else.if_block);
    auri_stmt_free(stmt->stmt.if_else.else_block);

    for(uint32_t i = 0; i < stmt->stmt.if_else.else_if_block.size; ++i) {
        auri_stmt_free(stmt->stmt.if_else.else_if_block.array[i]);
    }
    free_dynamic_ptr_array(&stmt->stmt.if_else.else_if_block);

    stmt->stmt.if_else.expr = NULL;
    stmt->stmt.if_else.if_block = NULL;
    stmt->stmt.if_else.else_block = NULL;
}

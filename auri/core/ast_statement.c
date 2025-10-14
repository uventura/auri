#include "auri/core/ast_statement.h"

#include <stdio.h>
#include <malloc.h>

AuriStmt* auri_stmt_init(AuriStmtType type, AuriStmtNode node) {
    AuriStmt* stmt = (AuriStmt*) malloc(sizeof(AuriStmt));
    stmt->type = type;
    stmt->stmt = node;
    return stmt;
}

void auri_stmt_free(AuriStmt* stmt) {
    switch(stmt->type) {
        case AST_STMT_EXPR:
            ast_node_free(stmt->stmt.expr.item);
            free(stmt->stmt.expr.item);
            break;
        case AST_STMT_RUN:
        case AST_STMT_IF:
        case AST_STMT_WHILE:
        case AST_STMT_FOR:
        case AST_STMT_VAR:
        case AST_STMT_IMPORT:
        case AST_STMT_RETURN:
        case AST_STMT_BREAK:
        case AST_STMT_CONTINUE:
            break;
    }
}

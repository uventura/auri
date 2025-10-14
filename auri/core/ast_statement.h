#ifndef AURI_AST_STATEMENTS_H
#define AURI_AST_STATEMENTS_H

#include "auri/core/ast_node.h"
#include "auri/utils/array.h"
#include "auri/utils/string.h"

typedef enum {
    AST_STMT_EXPR,
    AST_STMT_BLOCK,
    AST_STMT_RUN,
    AST_STMT_IF,
    AST_STMT_WHILE,
    AST_STMT_FOR,
    AST_STMT_VAR,
    AST_STMT_IMPORT,
    AST_STMT_RETURN,
    AST_STMT_BREAK,
    AST_STMT_CONTINUE
} AuriStmtType;

typedef enum {
    AST_SETUP_BLOCK,
    AST_PRERUN_BLOCK,
    AST_RUN_BLOCK,
    AST_POST_RUN_BLOCK,
    AST_END_BLOCK
} AuriStmtRunType;

typedef union {
    // Expression
    struct {
        AuriNode* item;
    } expr;
    // Block
    struct {
        DArrayVoidPtr items; // Statements - Run
    } block;
    // Run
    struct {
        AuriString name;
        AuriStmtRunType type;
        struct stmt* block;
    } run;
} AuriStmtNode;

typedef struct stmt {
    AuriStmtType type;
    AuriStmtNode stmt;
} AuriStmt;

AuriStmt* auri_stmt_init(AuriStmtType type, AuriStmtNode node);
void auri_stmt_free(AuriStmt* stmt);

#endif

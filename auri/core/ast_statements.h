#ifndef AURI_AST_STATEMENTS_H
#define AURI_AST_STATEMENTS_H

#include "auri/core/ast_node.h"
#include "auri/utils/array.h"
#include "auri/utils/string.h"

typedef enum {
    AST_STMT_EXPR,
    AST_STMT_RUN,
    AST_STMT_IF,
    AST_STMT_WHILE,
    AST_STMT_VAR,
    AST_STMT_IMPORT,
} AuriStmtType;

typedef struct {
    AuriNode* expression;
} AuriExprStmt;

typedef struct {
    AuriString name;
    DArrayVoidPtr expressions;
} AuriRunStmt;

#endif

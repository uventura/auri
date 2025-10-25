#ifndef AURI_AST_STATEMENTS_H
#include "auri/core/ast_node.h"
#include "auri/utils/array.h"
#include "auri/utils/string.h"

typedef enum {
    AST_STMT_NULL,
    AST_STMT_EXPR,
    AST_STMT_BLOCK,
    AST_STMT_RUN,
    AST_STMT_IF,
    AST_STMT_WHILE,
    AST_STMT_FOR,
    AST_STMT_VAR,
    AST_STMT_FUNCTION,
    AST_STMT_IMPORT,
    AST_STMT_RETURN,
    AST_STMT_BREAK,
    AST_STMT_CONTINUE
} AuriStmtType;

typedef union {
    // Expression
    struct {
        AuriNode* item;
    } expr;
    // Block
    struct {
        DArrayVoidPtr items; // Statements - Except "Run"
    } block;
    // Run
    struct {
        AuriToken* name;
        AuriTokenType type;
        struct stmt* block;
    } run;
    // If
    struct {
        AuriNode* expr;
        struct stmt* if_block;
        struct stmt* else_block;
        DArrayVoidPtr else_if_block;
    } if_else;
    // While
    struct {
        AuriNode* condition;
        struct stmt* block;
    } while_loop;
    // Variable
    // <type> identifier = <expression>;
    struct {
        AuriToken* type;
        AuriToken* identifier;
        struct stmt* expr;
    } var;
    // Function
    // fun[type] identifier(<parameters>) <block>
    struct {
        AuriToken* identifier;
        AuriToken* type;
        DArrayVoidPtr arguments;
        struct stmt* body;
    } function;
    // Return
    struct {
        AuriNode* value;
    } return_s;
} AuriStmtNode;

typedef struct stmt {
    AuriStmtType type;
    AuriStmtNode stmt;
} AuriStmt;

AuriStmt* auri_stmt_init(AuriStmtType type, AuriStmtNode node);
void auri_stmt_free(AuriStmt* stmt);

#endif

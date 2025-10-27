#include "auri/parser/parser.h"
#include "auri/parser/ast_node.h"
#include "auri/parser/ast_statement.h"
#include "auri/core/exceptions.h"

#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

AuriScanner* auri_parser_scanner;
uint32_t auri_parser_token_pos = 0;
bool auri_parser_is_function = false;
bool auri_parser_is_loop = false;

AuriStmt* init_declaration(void);
AuriStmt* declaration(void);

// Statements
AuriStmt* statement(void);
AuriStmt* import_stmt(void);
AuriStmt* expression_stmt(void);
AuriStmt* block_stmt(void);
AuriStmt* run_stmt(void);
AuriStmt* if_stmt(void);
AuriStmt* while_stmt(void);
AuriStmt* for_stmt(void);
AuriStmt* var_stmt(void);
AuriStmt* function_stmt(void);
AuriStmt* return_stmt(void);
AuriStmt* break_stmt(void);
AuriStmt* continue_stmt(void);

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
AuriNode* call(void);
AuriNode* function_call(AuriNode* node);
AuriNode* primary(void);

// Parser
bool parser_check_var_decl(void);

AuriToken* parser_peek(void);
AuriToken* parser_previous(void);
AuriToken* parser_advance(void);

bool parser_match(uint32_t size, ...);
bool parser_is_at_end(void);
AuriToken* parser_consume(char* message, uint32_t size, ...);

//||========================================================||
//||========================================================||
//||========================================================||

AuriAst* auri_parser(AuriScanner* scanner) {
    AuriAst* ast = (AuriAst*)malloc(sizeof(AuriAst));
    init_dynamic_ptr_array(&ast->statements);
    init_dynamic_ptr_array(&ast->run_statements);

    auri_parser_scanner = scanner;
    auri_parser_token_pos = 0;

    while(!parser_is_at_end()) {
        AuriStmt* stmt = init_declaration();
        if(stmt->type == AST_STMT_RUN) {
            insert_dynamic_ptr_array(&ast->run_statements, stmt);
        } else {
            insert_dynamic_ptr_array(&ast->statements, stmt);
        }
    }

    return ast;
}

void auri_parser_free(AuriAst* ast) {
    for(uint32_t i = 0; i < ast->statements.size; ++i){
        AuriStmt* stmt = ast->statements.array[i];
        auri_stmt_free(stmt);
    }
    for(uint32_t i = 0; i < ast->run_statements.size; ++i){
        AuriStmt* stmt = ast->run_statements.array[i];
        auri_stmt_free(stmt);
    }
    free_dynamic_ptr_array(&ast->statements);
    free_dynamic_ptr_array(&ast->run_statements);
    free(ast);
}

//||========================================================||
//||========================================================||
//||========================================================||

//+-------------+
//|  STATEMENTS |
//+-------------+

AuriStmt* init_declaration(void){
    if(parser_match(5, AR_TOKEN_RUN, AR_TOKEN_PRE_RUN, AR_TOKEN_POST_RUN, AR_TOKEN_SETUP, AR_TOKEN_MODULE)) return run_stmt();

    return declaration();
}

AuriStmt* declaration(void) {
    if(parser_check_var_decl()) return var_stmt();
    if(parser_match(1, AR_TOKEN_FUN)) return function_stmt();

    return statement();
}

AuriStmt* var_stmt(void) {
    AuriStmtNode var;
    var.var.type = parser_previous();
    var.var.identifier = parser_consume("The variable declaration is missing identifier", 1, AR_TOKEN_IDENTIFIER);
    var.var.expr = NULL;

    if(parser_match(1, AR_TOKEN_EQUAL))
        var.var.expr = expression_stmt();
    else
        parser_consume("Missing ';' in var declaration", 1, AR_TOKEN_SEMICOLON);

    return auri_stmt_init(AST_STMT_VAR, var);
}

AuriStmt* function_stmt(void){
    auri_parser_is_function = true;

    parser_consume("The function declaration is missing '[' in type definition", 1, AR_TOKEN_LEFT_BRACKET);
    parser_consume(
        "The function is missing type in declaration",
        6, AR_TOKEN_NUMERIC_VAR, AR_TOKEN_STRING_VAR, AR_TOKEN_BOOL_VAR, AR_TOKEN_NULL, AR_TOKEN_IDENTIFIER
    );
    AuriToken* type = parser_previous();

    parser_consume("The function is missing ']' in type definition", 1, AR_TOKEN_RIGHT_BRACKET);
    parser_consume("The function is missing identification", 1, AR_TOKEN_IDENTIFIER);

    AuriToken* identifier = parser_previous();
    parser_consume("The function is missing '(' to declare arguments", 1, AR_TOKEN_LEFT_PAREN);

    DArrayVoidPtr arguments;
    init_dynamic_ptr_array(&arguments);

    while(parser_peek()->type != AR_TOKEN_RIGHT_PAREN) {
        if(parser_check_var_decl()) {
            AuriStmtNode var;
            var.var.type = parser_previous();
            var.var.identifier = parser_consume("The variable declaration is missing identifier", 1, AR_TOKEN_IDENTIFIER);
            var.var.expr = NULL;

            if(parser_match(1, AR_TOKEN_EQUAL)) {
                AuriStmtNode expr_stmt;
                expr_stmt.expr.item = expression();
                var.var.expr = auri_stmt_init(AST_STMT_EXPR, expr_stmt);
            }

            AuriStmt* stmt = auri_stmt_init(AST_STMT_VAR, var);

            insert_dynamic_ptr_array(&arguments, stmt);
        } else {
            auri_throw_execution_error("Variable declaration missing type");
        }

        if(!parser_match(1, AR_TOKEN_COMMA)) {
            break;
        };
    }

    parser_consume("The function is missing ')' in argument declaration in function", 1, AR_TOKEN_RIGHT_PAREN);
    parser_consume("The function is missing '{' to declare its body", 1, AR_TOKEN_LEFT_BRACE);

    AuriStmt* body = block_stmt();

    AuriStmtNode fun;
    fun.function.type = type;
    fun.function.identifier = identifier;
    fun.function.arguments = arguments;
    fun.function.body = body;

    auri_parser_is_function = false;
    return auri_stmt_init(AST_STMT_FUNCTION, fun);
}

AuriStmt* statement(void) {
    if(parser_match(1, AR_TOKEN_IMPORT)) return import_stmt();
    else if(parser_match(1, AR_TOKEN_IF)) return if_stmt();
    else if(parser_match(1, AR_TOKEN_LEFT_BRACE)) return block_stmt();
    else if(parser_match(1, AR_TOKEN_WHILE)) return while_stmt();
    else if(parser_match(1, AR_TOKEN_FOR)) return for_stmt();
    else if(parser_match(1, AR_TOKEN_RETURN)) return return_stmt();
    else if(parser_match(1, AR_TOKEN_BREAK)) return break_stmt();
    else if(parser_match(1, AR_TOKEN_CONTINUE)) return continue_stmt();

    return expression_stmt();
}

AuriStmt* import_stmt(void) {
    return NULL;
}

AuriStmt* expression_stmt(void) {
    AuriNode* expr = expression();
    if(!parser_match(1, AR_TOKEN_SEMICOLON)) {
        auri_throw_execution_error("Expression missing ';' on line %d.\n", parser_peek()->line);
    }

    AuriStmtNode expr_stmt;
    expr_stmt.expr.item = expr;

    return auri_stmt_init(AST_STMT_EXPR, expr_stmt);
}

AuriStmt* block_stmt(void) {
    DArrayVoidPtr items;
    init_dynamic_ptr_array(&items);

    while(!parser_match(1, AR_TOKEN_RIGHT_BRACE)) {
        AuriStmt* stmt = declaration();
        insert_dynamic_ptr_array(&items, stmt);

        if(parser_is_at_end()) {
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
    AuriStmt* if_block = statement();
    AuriStmt* else_block = NULL;
    DArrayVoidPtr else_if_items;
    init_dynamic_ptr_array(&else_if_items);

    if(parser_match(1, AR_TOKEN_ELSE)) {
        else_block = statement();
    }

    AuriStmtNode if_stmt;
    if_stmt.if_else.expr = expr;
    if_stmt.if_else.if_block = if_block;
    if_stmt.if_else.else_block = else_block;
    if_stmt.if_else.else_if_block = else_if_items; // Not implemented yet.

    return auri_stmt_init(AST_STMT_IF, if_stmt);
}

AuriStmt* run_stmt(void) {
    AuriTokenType type = parser_previous()->type;

    if(!parser_match(1, AR_TOKEN_IDENTIFIER)) {
        auri_throw_execution_error("Run statement missing identification on line %d\n", parser_peek()->line);
    }

    AuriToken* name = parser_previous();
    if(!parser_match(1, AR_TOKEN_LEFT_BRACE)) {
        auri_throw_execution_error("Run statement missing '{' at start on line %d\n", parser_peek()->line);
    }

    AuriStmtNode run_stmt;
    run_stmt.run.name = name;
    run_stmt.run.block = block_stmt();
    run_stmt.run.type = type;

    return auri_stmt_init(AST_STMT_RUN, run_stmt);
}

AuriStmt* while_stmt(void) {
    auri_parser_is_loop = true;
    if(!parser_match(1, AR_TOKEN_LEFT_PAREN)) {
        auri_throw_execution_error("'while' missing '(' in condition.\n");
    }

    AuriNode* condition = expression();
    if(!parser_match(1, AR_TOKEN_RIGHT_PAREN)) {
        auri_throw_execution_error("'while' missing ')' in condition.\n");
    }

    AuriStmt* block = statement();
    AuriStmtNode while_loop;
    while_loop.while_loop.condition = condition;
    while_loop.while_loop.block = block;

    auri_parser_is_loop = false;
    return auri_stmt_init(AST_STMT_WHILE, while_loop);
}

AuriStmt* for_stmt(void) {
    auri_parser_is_loop = true;
    parser_consume("Missing '(' on for loop", 1, AR_TOKEN_LEFT_PAREN);

    AuriStmt* initializer = NULL;
    if(!parser_match(1, AR_TOKEN_SEMICOLON)) {
        initializer = expression_stmt();
    }

    AuriNode* condition = NULL;
    if(!parser_match(1, AR_TOKEN_SEMICOLON)) {
        condition = expression();
    }
    if(condition != NULL && !parser_match(1, AR_TOKEN_SEMICOLON)) {
        auri_throw_execution_error("Missing ';' after for condition on line %d.\n", parser_peek()->line);
    }

    AuriNode* increment = NULL;
    if(!parser_match(1, AR_TOKEN_RIGHT_PAREN)) {
        increment = expression();
    }
    if(increment != NULL && !parser_match(1, AR_TOKEN_RIGHT_PAREN)) {
        auri_throw_execution_error("Missing ')' on for loop in line %d\n", parser_peek()->line);
    }

    AuriStmtNode inc_stmt_node;
    inc_stmt_node.expr.item = increment;
    AuriStmt* inc_stmt = auri_stmt_init(AST_STMT_EXPR, inc_stmt_node);

    AuriStmt* block = statement();
    insert_dynamic_ptr_array(&block->stmt.block.items, inc_stmt);

    AuriStmtNode loop;
    loop.while_loop.condition = condition;
    loop.while_loop.block = block;
    AuriStmt* while_loop = auri_stmt_init(AST_STMT_WHILE, loop);

    DArrayVoidPtr for_loop;
    init_dynamic_ptr_array(&for_loop);
    insert_dynamic_ptr_array(&for_loop, initializer);
    insert_dynamic_ptr_array(&for_loop, while_loop);

    AuriStmtNode for_block;
    for_block.block.items = for_loop;

    auri_parser_is_loop = false;
    return auri_stmt_init(AST_STMT_BLOCK, for_block);
}

AuriStmt* return_stmt(void) {
    if(!auri_parser_is_function) {
        auri_throw_execution_error("Return statement ouside of function scope on line %d\n", parser_peek()->line + 1);
    }

    AuriStmtNode return_s;
    return_s.return_s.value = NULL;

    if(!parser_match(1, AR_TOKEN_SEMICOLON)) {
        return_s.return_s.value = expression();
    }

    parser_consume("Return statement missing ';'", 1, AR_TOKEN_SEMICOLON);
    return auri_stmt_init(AST_STMT_RETURN, return_s);
}

AuriStmt* break_stmt(void) {
    if(!auri_parser_is_loop) {
        auri_throw_execution_error("Break statement outside of loop scope on line %d.\n", parser_peek()->line + 1);
    }

    AuriStmtNode stmt;
    stmt.break_s.token = parser_peek();

    parser_consume("Break missing ';'", 1, AR_TOKEN_SEMICOLON);
    return auri_stmt_init(AST_STMT_BREAK, stmt);
}

AuriStmt* continue_stmt(void) {
    if(!auri_parser_is_loop) {
        auri_throw_execution_error("Continue statement outside of loop scope on line %d.\n", parser_peek()->line + 1);
    }

    AuriStmtNode stmt;
    stmt.continue_s.token = parser_peek();

    parser_consume("Continue missing ';'", 1, AR_TOKEN_SEMICOLON);
    return auri_stmt_init(AST_STMT_BREAK, stmt);
}


//+-------------+
//| EXPRESSIONS |
//+-------------+

#define BINARY_NODE(func, comparison_args_size, ...) \
    AuriNode* node = func();\
    AuriNode* current = node;\
    while(parser_match(comparison_args_size, __VA_ARGS__)) {\
        AuriToken* operator = parser_previous();\
        AuriNode* right = func();\
        current = ast_node_init(AST_NODE_BINARY, operator, current, right);\
    }\
    return current

AuriNode* expression(void) {
    return assignment();
}

AuriNode* assignment(void) {
    AuriNode* node = or();

    if(parser_match(1, AR_TOKEN_EQUAL)) {
        AuriToken* operator = parser_previous();
        node = ast_node_init(AST_NODE_BINARY, operator, node, assignment());
    }

    return node;
}

AuriNode* or(void) {
    AuriNode* node = and();
    AuriNode* current = node;

    while(parser_match(1, AR_TOKEN_OR)) {
        current->binary.left = ast_node_init(AST_NODE_BINARY, current->token, current->binary.left, current->binary.right);

        current->type = AST_NODE_BINARY;
        current->token = parser_previous();

        current->binary.right = equality();
        current = current->binary.right;
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
        return ast_node_init(AST_NODE_UNARY, operator, unary());
    }

    return call();
}

AuriNode* call(void) {
    AuriNode* node = primary();

    while(true) {
        if(parser_match(1, AR_TOKEN_LEFT_PAREN)) {
            node = function_call(node);
        } else {
            break;
        }
        // // No support for structs yet
        // else if(parser_match(1, AR_TOKEN_DOT)) {
        // }
    }

    return node;
}

AuriNode* function_call(AuriNode* node) {
    DArrayVoidPtr arguments;
    init_dynamic_ptr_array(&arguments);
    AuriToken* paren = parser_previous();

    while(parser_peek()->type != AR_TOKEN_RIGHT_PAREN && parser_peek()->type != AR_TOKEN_EOF) {
        AuriNode* argument = expression();
        insert_dynamic_ptr_array(&arguments, argument);

        if(parser_peek()->type != AR_TOKEN_RIGHT_PAREN && !parser_match(1, AR_TOKEN_COMMA)) {
            auri_throw_execution_error("Missing ',' o function call on line %d.\n", parser_previous()->line);
        }
    }

    parser_consume("Missing parenthesis on function call", 1, AR_TOKEN_RIGHT_PAREN);

    return ast_node_init(AST_NODE_CALL, paren, node, arguments);
}

AuriNode* primary(void) {
    if(parser_match(6, AR_TOKEN_NUMBER, AR_TOKEN_STRING, AR_TOKEN_TRUE, AR_TOKEN_FALSE, AR_TOKEN_NULL, AR_TOKEN_IDENTIFIER)) {
        return ast_node_init(AST_NODE_LITERAL, parser_previous());
    }

    if(parser_match(1, AR_TOKEN_LEFT_PAREN)) {
        AuriNode* expr = expression();
        parser_consume("No matching parenthesis", 1, AR_TOKEN_RIGHT_PAREN);

        return expr;
    }

    auri_throw_execution_error("Wrong token usage (Line: %d)[%s]: %s\n",
        parser_peek()->line + 1, auri_token_name(parser_peek()->type), parser_peek()->lexeme.text);
    return NULL;
}

//+-------------+
//|    PARSER   |
//+-------------+
bool parser_check_var_decl(void) {
    return parser_match(
        4,
        AR_TOKEN_GENERIC_VAR,
        AR_TOKEN_NUMERIC_VAR,
        AR_TOKEN_STRING_VAR,
        AR_TOKEN_BOOL_VAR);
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
    if(auri_parser_token_pos <= 0) {
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

bool parser_is_at_end(void) {
    return parser_peek()->type == AR_TOKEN_EOF;
}

AuriToken* parser_consume(char* message, uint32_t size, ...) {
    va_list args;
    va_start(args, size);

    for(uint32_t i = 0; i < size; ++i) {
        AuriTokenType type = va_arg(args, AuriTokenType);
        if(type == parser_peek()->type) {
            va_end(args);
            AuriToken* current = parser_peek();
            parser_advance();
            return current;
        }
    }
    va_end(args);

    printf("%s", message);
    auri_throw_execution_error(" on line %d.\n", parser_peek()->line + 1);

    return NULL;
}

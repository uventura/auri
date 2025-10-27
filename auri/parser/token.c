#include "auri/parser/token.h"

#include "auri/utils/string.h"

#include <malloc.h>
#include <stdio.h>

const uint32_t AuriTokenIdentifiersSize = 26;
const char*  AuriTokenIdentifiers[] = {
    "var",
    "numeric",
    "string",
    "bool",
    "if",
    "else",
    "elseif",
    "for",
    "while",
    "return",
    "true",
    "false",
    "or",
    "and",
    "alias",
    "as",
    "import",
    "fun",
    "null",
    "break",
    "continue",
    "setup",
    "prerun",
    "run",
    "postrun",
    "module",
};

AuriToken* auri_token_init(AuriString lexeme, AuriLiteral literal, AuriTokenType type, uint32_t line) {
    AuriToken* token = (AuriToken*)malloc(sizeof(AuriToken));
    token->lexeme = lexeme;
    token->line = line;
    token->literal = literal;
    token->type = type;

    return token;
}

void auri_token_free(AuriToken* token) {
    if(token == NULL) return;

    auri_strfree(&token->lexeme);
    auri_strfree(&token->literal.string);
    free(token);
}

const char* auri_token_name(AuriTokenType token_type) {
    switch(token_type) {
        case AR_TOKEN_LEFT_PAREN:
            return "LEFT_PAREN";
            break;
        case AR_TOKEN_RIGHT_PAREN:
            return "RIGHT_PAREN";
            break;
        case AR_TOKEN_LEFT_BRACE:
            return "LEFT_BRACE";
            break;
        case AR_TOKEN_RIGHT_BRACE:
            return "RIGHT_BRACE";
            break;
        case AR_TOKEN_LEFT_BRACKET:
            return "LEFT_BRACKET";
            break;
        case AR_TOKEN_RIGHT_BRACKET:
            return "RIGHT_BRACKET";
            break;
        case AR_TOKEN_COMMA:
            return "COMMA";
            break;
        case AR_TOKEN_DOT:
            return "DOT";
            break;
        case AR_TOKEN_MINUS:
            return "MINUS";
            break;
        case AR_TOKEN_PLUS:
            return "PLUS";
            break;
        case AR_TOKEN_SEMICOLON:
            return "SEMICOLON";
            break;
        case AR_TOKEN_SLASH:
            return "SLASH";
            break;
        case AR_TOKEN_STAR:
            return "STAR";
            break;
        case AR_TOKEN_BANG:
            return "BANG";
            break;
        case AR_TOKEN_BANG_EQUAL:
            return "BANG_EQUAL";
            break;
        case AR_TOKEN_EQUAL:
            return "EQUAL";
            break;
        case AR_TOKEN_EQUAL_EQUAL:
            return "EQUAL_EQUAL";
            break;
        case AR_TOKEN_GREATER:
            return "GREATER";
            break;
        case AR_TOKEN_GREATER_EQUAL:
            return "GREATER_EQUAL";
            break;
        case AR_TOKEN_LESS:
            return "LESS";
            break;
        case AR_TOKEN_LESS_EQUAL:
            return "LESS_EQUAL";
            break;
        case AR_TOKEN_STRING:
            return "STRING";
            break;
        case AR_TOKEN_NUMBER:
            return "NUMBER";
            break;
        case AR_TOKEN_IDENTIFIER:
            return "IDENTIFIER";
            break;
        case AR_TOKEN_GENERIC_VAR:
            return "GENERIC_VAR";
            break;
        case AR_TOKEN_NUMERIC_VAR:
            return "NUMERIC_VAR";
            break;
        case AR_TOKEN_STRING_VAR:
            return "STRING_VAR";
            break;
        case AR_TOKEN_BOOL_VAR:
            return "BOOL_VAR";
            break;
        case AR_TOKEN_IF:
            return "IF";
            break;
        case AR_TOKEN_ELSE:
            return "ELSE";
            break;
        case AR_TOKEN_ELSE_IF:
            return "ELSE_IF";
            break;
        case AR_TOKEN_FOR:
            return "FOR";
            break;
        case AR_TOKEN_WHILE:
            return "WHILE";
            break;
        case AR_TOKEN_RETURN:
            return "RETURN";
            break;
        case AR_TOKEN_TRUE:
            return "TRUE";
            break;
        case AR_TOKEN_FALSE:
            return "FALSE";
            break;
        case AR_TOKEN_OR:
            return "OR";
            break;
        case AR_TOKEN_AND:
            return "AND";
            break;
        case AR_TOKEN_ALIAS:
            return "ALIAS";
            break;
        case AR_TOKEN_AS:
            return "AS";
            break;
        case AR_TOKEN_IMPORT:
            return "IMPORT";
            break;
        case AR_TOKEN_FUN:
            return "FUN";
            break;
        case AR_TOKEN_NULL:
            return "AR_NULL";
            break;
        case AR_TOKEN_BREAK:
            return "AR_BREAK";
            break;
        case AR_TOKEN_CONTINUE:
            return "AR_CONTINUE";
            break;
        case AR_TOKEN_SETUP:
            return "SETUP";
            break;
        case AR_TOKEN_PRE_RUN:
            return "PRE_RUN";
            break;
        case AR_TOKEN_RUN:
            return "RUN";
            break;
        case AR_TOKEN_POST_RUN:
            return "POST_RUN";
            break;
        case AR_TOKEN_MODULE:
            return "AR_MODULE";
            break;
        case AR_TOKEN_EOF:
            return "EOF";
            break;
        case AR_TOKEN_NONE:
            return "NONE";
            break;
    }

    return "<UNDEFINED>";
}

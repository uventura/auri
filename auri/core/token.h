#ifndef AURI_TOKENS_H
#define AURI_TOKENS_H

#include "auri/core/common.h"
#include "auri/utils/string.h"

#include <uchar.h>

#define AURI_TOKEN_IDENTIFIER_START 1000

typedef enum {
    // Math Operators
    AR_TOKEN_LEFT_PAREN,
    AR_TOKEN_RIGHT_PAREN,
    AR_TOKEN_LEFT_BRACE,
    AR_TOKEN_RIGHT_BRACE,
    AR_TOKEN_COMMA,
    AR_TOKEN_DOT,
    AR_TOKEN_MINUS,
    AR_TOKEN_PLUS,
    AR_TOKEN_SEMICOLON,
    AR_TOKEN_SLASH,
    AR_TOKEN_STAR,
    AR_TOKEN_BANG,
    AR_TOKEN_BANG_EQUAL,
    AR_TOKEN_EQUAL,
    AR_TOKEN_EQUAL_EQUAL,
    AR_TOKEN_GREATER,
    AR_TOKEN_GREATER_EQUAL,
    AR_TOKEN_LESS,
    AR_TOKEN_LESS_EQUAL,
    // Literals
    AR_TOKEN_STRING,
    AR_TOKEN_NUMBER,
    AR_TOKEN_IDENTIFIER,
    // Variables
    AR_TOKEN_GENERIC_VAR = AURI_TOKEN_IDENTIFIER_START,
    AR_TOKEN_NUMERIC_VAR,
    AR_TOKEN_STRING_VAR,
    AR_TOKEN_BOOL_VAR,
    // Keywords
    AR_TOKEN_IF,
    AR_TOKEN_ELSE,
    AR_TOKEN_ELSE_IF,
    AR_TOKEN_FOR,
    AR_TOKEN_WHILE,
    AR_TOKEN_RETURN,
    AR_TOKEN_TRUE,
    AR_TOKEN_FALSE,
    AR_TOKEN_OR,
    AR_TOKEN_AND,
    AR_TOKEN_ALIAS,
    AR_TOKEN_AS,
    AR_TOKEN_IMPORT,
    AR_TOKEN_FUN,
    AR_TOKEN_NULL,
    // Execution Control
    AR_TOKEN_SETUP,
    AR_TOKEN_PRE_RUN,
    AR_TOKEN_RUN,
    AR_TOKEN_POST_RUN,
    // EOF
    AR_TOKEN_EOF,
    // NONE
    AR_TOKEN_NONE
} AuriTokenType;

extern const uint32_t AuriTokenIdentifiersSize;
extern const char*  AuriTokenIdentifiers[];

typedef union {
    AuriString string;
    double numeric;
    bool boolean;
} AuriLiteral;

typedef struct {
    AuriString lexeme;
    uint32_t line;
    AuriTokenType type;
    AuriLiteral literal;
} AuriToken;

AuriToken* auri_token_init(AuriString lexeme, AuriLiteral literal, AuriTokenType type, uint32_t line);
void auri_token_free(AuriToken* token);
const char* auri_token_name(AuriTokenType token_type);

#endif

#include "auri/core/scanner.h"
#include "auri/core/common.h"
#include "auri/utils/string.h"
#include "auri/utils/debug.h"

#include <stdio.h>
#include <uchar.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>

AuriString* auri_scanner_file;
uint32_t auri_scanner_line = 0;
uint32_t auri_scanner_current_position = 0;

void append_token(AuriScanner* scanner, AuriToken* token);
char* read_file(const char* path);

void text(AuriString* lexeme, AuriLiteral* literal);
void digit(AuriString* lexeme);
AuriTokenType identifier(AuriString* lexeme, AuriLiteral* literal);

AuriToken* scanner_eof_token(void);

bool scanner_match(char symbol);
char scanner_peek(void);
char scanner_back(void);
char scanner_advance(void);
bool scanner_eof(void);

AuriScanner auri_scanner(const char* path) {
    auri_scanner_current_position = 0;
    auri_scanner_line = 0;

    AuriScanner scanner;
    init_dynamic_ptr_array(&scanner.tokens);

    char* buffer = read_file(path);
    auri_scanner_file = (AuriString*)malloc(sizeof(AuriString));
    auri_strinit(auri_scanner_file);
    auri_strcat(auri_scanner_file, buffer, strlen(buffer));
    free(buffer);

    char symbol = scanner_peek();

    while(symbol) {
        AuriTokenType type = AR_TOKEN_NONE;

        AuriLiteral literal;
        auri_strinit(&literal.string);
        auri_strcat(&literal.string, &symbol, 1);

        AuriString lexeme;
        auri_strinit(&lexeme);
        auri_strcat(&lexeme, &symbol, 1);

        switch(symbol) {
            case '(':
                type = AR_TOKEN_LEFT_PAREN;
                break;
            case ')':
                type = AR_TOKEN_RIGHT_PAREN;
                break;
            case '{':
                type = AR_TOKEN_LEFT_BRACE;
                break;
            case '}':
                type = AR_TOKEN_RIGHT_BRACE;
                break;
            case '[':
                type = AR_TOKEN_LEFT_BRACKET;
                break;
            case ']':
                type = AR_TOKEN_RIGHT_BRACKET;
                break;
            case ',':
                type = AR_TOKEN_COMMA;
                break;
            case '.':
                type = AR_TOKEN_DOT;
                break;
            case '-':
                type = AR_TOKEN_MINUS;
                break;
            case '+':
                type = AR_TOKEN_PLUS;
                break;
            case ';':
                type = AR_TOKEN_SEMICOLON;
                break;
            case '/':
                type = AR_TOKEN_SLASH;
                break;
            case '*':
                type = AR_TOKEN_STAR;
                break;
            case '!': {
                type = AR_TOKEN_BANG;
                if (scanner_match('=')) {
                    type = AR_TOKEN_BANG_EQUAL;
                    auri_strcat(&lexeme, "=", 1);
                }
                break;
            }
            case '=': {
                type = AR_TOKEN_EQUAL;
                if (scanner_match('=')) {
                    type = AR_TOKEN_EQUAL_EQUAL;
                    auri_strcat(&lexeme, "=", 1);
                }
                break;
            }
            case '>': {
                type = AR_TOKEN_GREATER;
                if (scanner_match('=')) {
                    type = AR_TOKEN_GREATER_EQUAL;
                    auri_strcat(&lexeme, "=", 1);
                }
                break;
            }
            case '<': {
                type = AR_TOKEN_LESS;
                if (scanner_match('=')) {
                    type = AR_TOKEN_LESS_EQUAL;
                    auri_strcat(&lexeme, "=", 1);
                }
                break;
            }
            case '"': {
                type = AR_TOKEN_STRING;
                text(&lexeme, &literal);
                break;
            }
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                auri_scanner_line++;
                break;
            default: {
                if (isdigit(symbol)) {
                    type = AR_TOKEN_NUMBER;
                    digit(&lexeme);

                    auri_strfree(&literal.string);
                    literal.numeric = strtod(lexeme.text, NULL);
                } else if(isalpha(symbol)) {
                    type = identifier(&lexeme, &literal);
                }
                else {
                    auri_throw_execution_error(
                        "In the file '%s' there is an unexpected element '%s' on auri_scanner_line %d\n", path, lexeme.text, auri_scanner_line
                    );
                }
                break;
            }
        }
        symbol = scanner_advance();
        if(type != AR_TOKEN_NONE) {
            AuriToken* token = auri_token_init(lexeme, literal, type, auri_scanner_line);
            append_token(&scanner, token);
        } else {
            auri_strfree(&lexeme);
            auri_strfree(&literal.string);
        }
    }

    append_token(&scanner, scanner_eof_token());

    auri_strfree(auri_scanner_file);
    free(auri_scanner_file);

    return scanner;
}

void auri_print_tokens(AuriScanner* scanner) {
    printf("=========================\n");
    printf("|         TOKENS        |\n");
    printf("=========================\n");

    for(uint32_t i = 0; i < scanner->tokens.size; ++i) {
        AuriToken* token = scanner->tokens.array[i];
        printf("| Token[%d]: (auri_scanner_line: %04d) <%s>:: '%s'\n", i, token->line + 1, auri_token_name(token->type), token->lexeme.text);
    }

    printf("=========================\n");
}

void auri_scanner_free(AuriScanner* scanner) {
    for(uint32_t i = 0; i < scanner->tokens.size; ++i){
        auri_token_free(scanner->tokens.array[i]);
    }
    free_dynamic_ptr_array(&scanner->tokens);
}

void append_token(AuriScanner* scanner, AuriToken* token) {
    insert_dynamic_ptr_array(&scanner->tokens, token);
}

char* read_file(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        auri_throw_execution_error("The file '%s' can't be opened.\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(sizeof(char) * (length + 1));
    if (!buffer) {
        auri_throw_execution_error("The file '%s' contains memory errors", path);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    rewind(file);
    fclose(file);

    return buffer;
}

void text(AuriString* lexeme, AuriLiteral* literal) {
    auri_strclear(&literal->string);

    char previous = scanner_peek();
    char current = scanner_advance();
    uint32_t line = auri_scanner_line;

    while(current != '"' && !scanner_eof()) {
        auri_strcat(lexeme, &current, 1);
        auri_strcat(&literal->string, &current, 1);

        previous = scanner_peek();
        current = scanner_advance();

        if(current == '"' && previous == '\\') {
            auri_strcat(&literal->string, &current, 1);
            previous = scanner_peek();
            current = scanner_advance();
        } else if(current == '\n') {
            auri_scanner_line++;
        }
    }

    if(scanner_peek() != '"') {
        auri_throw_execution_error("The string on line %d doesn't have an end.\n", line + 1);
    }

    auri_strcat(lexeme, "\"", 1);
}

void digit(AuriString* lexeme) {
    char symbol = scanner_advance();
    while(!scanner_eof() && isdigit(symbol)) {
        auri_strcat(lexeme, &symbol, 1);
        symbol = scanner_advance();
    }

    if(!scanner_eof() && symbol == '.') {
        auri_strcat(lexeme, &symbol, 1);
        symbol = scanner_advance();
        while(!scanner_eof() && isdigit(symbol)) {
            auri_strcat(lexeme, &symbol, 1);
            symbol = scanner_advance();
        }
    }

    scanner_back();
}

AuriTokenType identifier(AuriString* lexeme, AuriLiteral* literal) {
    char symbol = scanner_advance();

    while((isalnum(symbol) || symbol == '_') && !scanner_eof()) {
        auri_strcat(lexeme, &symbol, 1);
        auri_strcat(&literal->string, &symbol, 1);
        symbol = scanner_advance();
    }

    scanner_back();
    for(uint32_t i = 0; i < AuriTokenIdentifiersSize; ++i) {
        if(strcmp(lexeme->text, AuriTokenIdentifiers[i]) == 0) {
            return AURI_TOKEN_IDENTIFIER_START + i;
        }
    }

    return AR_TOKEN_IDENTIFIER;
}

AuriToken* scanner_eof_token(void) {
    AuriString lexeme;
    lexeme.text = NULL;

    AuriLiteral literal;
    literal.numeric = 0;

    return auri_token_init(lexeme, literal, AR_TOKEN_EOF, auri_scanner_line);
}

bool scanner_match(char symbol) {
    if(scanner_eof()) {
        return false;
    }

    if(auri_scanner_file->text[auri_scanner_current_position + 1] == symbol) {
        scanner_advance();
        return true;
    }

    return false;
}

char scanner_peek(void) {
    return auri_strchar(auri_scanner_file, auri_scanner_current_position);
}

char scanner_back(void) {
    if(auri_scanner_current_position > 0) {
        --auri_scanner_current_position;
        return scanner_peek();
    }

    return scanner_peek();
}

char scanner_advance(void) {
    if(!scanner_eof()) {
        ++auri_scanner_current_position;
        return scanner_peek();
    }
    return '\0';
}

bool scanner_eof(void) {
    return auri_scanner_current_position >= auri_scanner_file->size;
}

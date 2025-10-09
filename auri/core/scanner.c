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

AuriString* auri_file;
uint32_t current_position = 0;

void append_token(AuriScanner* scanner, AuriString lexeme, AuriString literal, AuriTokenType type, uint32_t line);

char* read_file(const char* path);

void text(AuriString* lexeme, AuriString* literal);
bool match(char symbol);
char peek();
char advance();

AuriScanner auri_scanner(const char* path) {
    current_position = 0;

    AuriScanner scanner;
    init_dynamic_ptr_array(&scanner.tokens, TOKEN_TYPE);

    char* buffer = read_file(path);
    auri_file = (AuriString*)malloc(sizeof(AuriString));
    auri_strinit(auri_file);
    auri_strcat(auri_file, buffer, strlen(buffer));
    free(buffer);

    char symbol = peek();
    uint32_t line = 0;

    while(symbol) {
        AuriTokenType type = AR_TOKEN_NONE;

        AuriString literal;
        auri_strinit(&literal);
        auri_strcat(&literal, &symbol, 1);

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
                if (match('=')) {
                    type = AR_TOKEN_BANG_EQUAL;
                    auri_strcat(&lexeme, "=", 1);
                }
                break;
            }
            case '=': {
                type = AR_TOKEN_EQUAL;
                if (match('=')) {
                    type = AR_TOKEN_EQUAL_EQUAL;
                    auri_strcat(&lexeme, "=", 1);
                }
                break;
            }
            case '>': {
                type = AR_TOKEN_GREATER;
                if (match('=')) {
                    type = AR_TOKEN_GREATER_EQUAL;
                    auri_strcat(&lexeme, "=", 1);
                }
                break;
            }
            case '<': {
                type = AR_TOKEN_LESS;
                if (match('=')) {
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
                line++;
                break;
            default: {
                // if (isdigit(symbol)) {
                //     type = AR_TOKEN_NUMBER;
                //     digit(lexeme);
                //     literal = stod(lexeme);
                // } else if (isalpha(symbol)) {
                //     identifier(lexeme, type);
                //     literal = lexeme;
                // } else {
                //     auri_throw_execution_error(
                //         "In the file '%s' there is an unexpected element '%s' on line %d", path, lexeme.text, line
                //     );
                // }
                break;
            }
        }
        symbol = advance();
        if(type != AR_TOKEN_NONE) {
            append_token(&scanner, lexeme, literal, type, line);
        } else {
            auri_strfree(&lexeme);
            auri_strfree(&literal);
        }
    }

    auri_strfree(auri_file);
    free(auri_file);

    return scanner;
}

void auri_print_tokens(AuriScanner* scanner) {
    printf("=========================\n");
    printf("|         TOKENS        |\n");
    printf("=========================\n");

    for(uint32_t i = 0; i < scanner->tokens.size; ++i) {
        AuriToken* token = scanner->tokens.array[i];
        printf("| Token: (Line: %04d) '%s'\n", token->line, token->lexeme.text);
    }

    printf("=========================\n");
}

void auri_scanner_free(AuriScanner* scanner) {
    for(uint32_t i = 0; i < scanner->tokens.size; ++i){
        auri_token_free(scanner->tokens.array[i]);
    }
    free_dynamic_ptr_array(&scanner->tokens);
}

void append_token(AuriScanner* scanner, AuriString lexeme, AuriString literal, AuriTokenType type, uint32_t line) {
    AuriToken* token = (AuriToken*)malloc(sizeof(AuriToken));
    token->type = type;
    token->lexeme = lexeme;
    token->literal = literal;
    token->line = line;
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

void text(AuriString* lexeme, AuriString* literal) {
    auri_strclear(literal);

    char previous = peek();
    char current = advance();

    while(current != '"' && previous != '\\') {
        auri_strcat(lexeme, &current, 1);
        auri_strcat(literal, &current, 1);

        previous = peek();
        current = advance();
    }

    auri_strcat(lexeme, "\"", 1);
}

bool match(char symbol) {
    if(current_position + 1 >= auri_file->size) {
        return false;
    }

    if(auri_file->text[current_position + 1] == symbol) {
        advance();
        return true;
    }

    return false;
}

char peek() {
    return auri_strchar(auri_file, current_position);
}

char advance() {
    if(current_position + 1 < auri_file->size) {
        current_position++;
        return peek();
    }
    return '\0';
}

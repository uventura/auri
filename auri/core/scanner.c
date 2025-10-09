#include "auri/core/scanner.h"
#include "auri/core/common.h"
#include "auri/utils/string.h"
#include "auri/utils/debug.h"

#include <stdio.h>
#include <uchar.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>

AuriString* auri_file;
uint32_t current_position = 0;

void append_token(AuriScanner* scanner, AuriString lexeme, AuriString literal, AuriTokenType type, uint32_t line);

void read_file(const char* path);
bool match(char symbol);
char peek();
char advance();

AuriScanner auri_scan(const char* path) {
    auri_debug_fprint("==========================\n");
    auri_debug_fprint("|         SCANNER        |\n");
    auri_debug_fprint("==========================\n");
    AuriScanner scanner;
    init_dynamic_ptr_array(&scanner.tokens, TOKEN_TYPE);

    auri_file = (AuriString*)malloc(sizeof(AuriString));
    auri_strinit(auri_file);
    read_file(path);

    char symbol = peek();
    uint32_t line = 0;

    while(symbol) {
        AuriTokenType type = AR_TOKEN_NONE;

        AuriString lexeme;
        auri_strinit(&lexeme);

        auri_debug_fprint("Next token\n");
        auri_strcat(&lexeme, &symbol, 1);
        printf(">>> %s\n", lexeme.text);

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
                // type = AR_TOKEN_STRING;
                // text(lexeme, line);
                // lexeme += '"';
                // literal = lexeme.substr(1, lexeme.size() - 2);
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
                //         "In the file '%s' there is an unexpected element '%s' on line %d",
                //     );
                //     throw std::invalid_argument(
                //         "In the file '" + filePath_ +
                //         "' there is an unexpected element '" + lexeme +
                //         "' on line " + std::to_string(line));
                // }
                break;
            }
        }
        symbol = advance();

        if(type != AR_TOKEN_NONE) {
            auri_debug_fprint("Adding new token");
            append_token(&scanner, lexeme, lexeme, type, line);
        }

        auri_strfree(&lexeme);
    }

    for(uint32_t i = 0; i < scanner.tokens.size; ++i) {
        AuriToken* token = scanner.tokens.array[i];
        printf(">> %s\n", token->lexeme.text);
    }

    auri_strfree(auri_file);

    return scanner;
}

void append_token(AuriScanner* scanner, AuriString lexeme, AuriString literal, AuriTokenType type, uint32_t line) {
    AuriToken* token = (AuriToken*)malloc(sizeof(AuriToken));
    token->type = type;
    token->lexeme = lexeme;
    token->line = line;
    insert_dynamic_ptr_array(&scanner->tokens, token);
}

void read_file(const char* path) {
    auri_debug_fprint("Opening file %s...\n", path);

    FILE* file = fopen(path, "r");
    if (!file) {
        auri_throw_execution_error("The file '%s' can't be opened", path);
        return;
    }

    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(sizeof(char) * (length + 1));
    if (!buffer) {
        auri_throw_execution_error("The file '%s' contains memory errors", path);
        return;
    }

    fread(buffer, 1, length, file);

    buffer[length] = '\0';
    auri_strcat(auri_file, buffer, length + 1);

    free(buffer);
    fclose(file);
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

#include "auri/core/scanner.h"
#include "auri/core/common.h"
#include "auri/utils/string.h"

#include <stdio.h>
#include <uchar.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>

FILE* auri_file;

bool match(char32_t symbol);
char32_t peek();
char32_t advance();

AuriScanner auri_scan(char* path) {
    AuriScanner scanner;
    init_dynamic_ptr_array(&scanner.tokens, TOKENS);

    auri_file = fopen(path, "r");
    if(!auri_file) {
        auri_throw_execution_error("The file '%s' contains errors", path);
    }

    char32_t symbol = fgetc(auri_file);
    AuriTokenType type = AR_TOKEN_NONE;

    AuriString lexeme;
    auri_strinit(&lexeme);

    uint32_t line = 0;

    while(symbol) {
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
                    auri_strcat(&lexeme, L"=");
                }
                break;
            }
            case '=': {
                type = AR_TOKEN_EQUAL;
                if (match('=')) {
                    type = AR_TOKEN_EQUAL_EQUAL;
                    auri_strcat(&lexeme, L"=");
                }
                break;
            }
            case '>': {
                type = AR_TOKEN_GREATER;
                if (match('=')) {
                    type = AR_TOKEN_GREATER_EQUAL;
                    auri_strcat(&lexeme, L"=");
                }
                break;
            }
            case '<': {
                type = AR_TOKEN_LESS;
                if (match('=')) {
                    type = AR_TOKEN_LESS_EQUAL;
                    auri_strcat(&lexeme, L"=");
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
        symbol = fgetc(auri_file);

        AuriToken* token = (AuriToken*)malloc(sizeof(AuriToken));
        token->type = type;
        insert_dynamic_ptr_array(&scanner.tokens, token, 1);
        //void insert_dynamic_ptr_array(DArrayVoidPtr* array, void* element, uint32_t size);
    }

    return scanner;
}

bool match(char32_t symbol) {
    return symbol == U'A';
}

char32_t peek() {
    return (char32_t) U'H';
}

char32_t advance() {
    return (char32_t) U'H';
}

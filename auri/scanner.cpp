#include "auri/scanner.hpp"

#include <cstdint>
#include <stdexcept>
#include <string>
#include <iostream>

namespace Auri {
Scanner::Scanner(std::string filePath) {
    file_.open(filePath);
    filePath_ = filePath;

    if (!file_.is_open()) {
        throw std::invalid_argument("The file '" + filePath +
                                    "' can't be found.");
    }

    tokenize();
}

void Scanner::tokenize() {
    char fileChar = advance();
    uint32_t line = 0;

    while (fileChar) {
        std::string lexeme(1, fileChar);
        TokenType type = TokenType::NONE;
        TokenLiteral literal = lexeme;

        switch (peek()) {
            case '(':
                type = TokenType::LEFT_PAREN;
                break;
            case ')':
                type = TokenType::RIGHT_PAREN;
                break;
            case '{':
                type = TokenType::LEFT_BRACE;
                break;
            case '}':
                type = TokenType::RIGHT_BRACE;
                break;
            case ',':
                type = TokenType::COMMA;
                break;
            case '.':
                type = TokenType::DOT;
                break;
            case '-':
                type = TokenType::MINUS;
                break;
            case '+':
                type = TokenType::PLUS;
                break;
            case ';':
                type = TokenType::SEMICOLON;
                break;
            case '/':
                type = TokenType::SLASH;
                break;
            case '*':
                type = TokenType::STAR;
                break;
            case '!': {
                type = TokenType::BANG;
                if (match('=')) {
                    type = TokenType::BANG_EQUAL;
                    lexeme += '=';
                }
                break;
            }
            case '=': {
                type = TokenType::EQUAL;
                if (match('=')) {
                    type = TokenType::EQUAL_EQUAL;
                    lexeme += '=';
                }
                break;
            }
            case '>': {
                type = TokenType::GREATER;
                if (match('=')) {
                    type = TokenType::GREATER_EQUAL;
                    lexeme += '=';
                }
                break;
            }
            case '<': {
                type = TokenType::LESS;
                if (match('=')) {
                    type = TokenType::LESS_EQUAL;
                    lexeme += '=';
                }
                break;
            }
            case '"': {
                type = TokenType::STRING;
                char current = advance();
                while(!file_.eof() && current != '"') {
                    lexeme += peek();
                    current = advance();
                }

                if(file_.eof()) {
                    throw std::invalid_argument("In file '" + filePath_ +
                                    "' there is an unclosed string on line " + std::to_string(line) + " called '" + lexeme + "'");
                }

                lexeme += '"';
                literal = lexeme.substr(1,lexeme.size() - 2);
            }
            case '\n':
                line++;
            default:
                break;
        }

        if (type != TokenType::NONE) {
            tokens_.push_back(Token(type, lexeme, literal, line));
        }

        fileChar = advance();
    }
}

char Scanner::peek() {
    if(file_.eof()) return 0;

    char current = 0;
    file_.unget();
    file_.get(current);

    return current;
}

char Scanner::advance() {
    if (file_.eof()) return 0;

    char current = 0;
    file_.get(current);

    return current;
}

bool Scanner::match(char symbol) {
    if (file_.eof()) return false;

    if (symbol != advance()) {
        file_.unget();
        return false;
    };

    return true;
}
}    // namespace Auri

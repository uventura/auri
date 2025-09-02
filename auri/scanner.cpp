#include "auri/scanner.hpp"

#include <cctype>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

namespace Auri {
std::map<std::string, TokenType> Scanner::keywords_ = {
    {"var", TokenType::GENERIC_VAR},
    {"numeric", TokenType::NUMERIC_VAR},
    {"string", TokenType::STRING_VAR},
    {"bool", TokenType::BOOL_VAR},
    {"null", TokenType::AR_NULL},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"elseif", TokenType::ELSE_IF},
    {"for", TokenType::FOR},
    {"while", TokenType::WHILE},
    {"return", TokenType::RETURN},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"or", TokenType::OR},
    {"and", TokenType::AND},
    {"alias", TokenType::ALIAS},
    {"as", TokenType::AS},
    {"import", TokenType::IMPORT},
    {"fun", TokenType::FUN},
    {"setup", TokenType::SETUP},
    {"prerun", TokenType::PRE_RUN},
    {"run", TokenType::RUN},
    {"postrun", TokenType::POST_RUN}};

Scanner::Scanner(std::string filePath) {
    file_.open(filePath);
    filePath_ = filePath;

    if (!file_.is_open()) {
        throw std::invalid_argument("The file '" + filePath +
                                    "' can't be found.");
    }

    tokenize();
}

void Scanner::debugPrint() {
    std::cout << "*------------------------------------------*" << std::endl;
    std::cout << "|                  Scanner                 |" << std::endl;
    std::cout << "*------------------------------------------*" << std::endl;
    for (auto i : tokens()) {
        std::cout << "|  Line " << i.line() << ": [" << i.typeToString()
                  << "] .... Lexeme: " << i.lexeme()
                  << " .... Literal: " << i.literal() << "\n";
    }
    std::cout << "*------------------------------------------*" << std::endl;
    std::cout << "*------------------------------------------*" << std::endl;
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
                while (!file_.eof() && current != '"') {
                    lexeme += peek();
                    current = advance();

                    if (peek() == '\n') line++;
                }

                if (file_.eof()) {
                    throw std::invalid_argument(
                        "In file '" + filePath_ +
                        "' there is an unclosed string on line " +
                        std::to_string(line) + " called '" + lexeme + "'");
                }

                lexeme += '"';
                literal = lexeme.substr(1, lexeme.size() - 2);
            }
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                line++;
                break;
            default: {
                if (std::isdigit(fileChar)) {
                    type = TokenType::NUMBER;
                    char current = advance();
                    while (!file_.eof() && std::isdigit(current)) {
                        lexeme += peek();
                        current = advance();
                    }

                    if (peek() == '.' && std::isdigit(peekNext())) {
                        advance();    // consume dot.
                        lexeme += ".";

                        char current = advance();
                        while (std::isdigit(current)) {
                            lexeme += current;
                            current = advance();
                        }
                    }
                    literal = std::stod(lexeme);
                } else if (std::isalpha(fileChar)) {
                    char current = advance();
                    while (std::isalpha(current) || std::isdigit(current) ||
                           current == '_') {
                        lexeme += current;
                        current = advance();
                    }
                    file_.unget();

                    if (Scanner::keywords_.find(lexeme) !=
                        Scanner::keywords_.end()) {
                        type = Scanner::keywords_[lexeme];
                    } else {
                        type = TokenType::IDENTIFIER;
                    }

                    literal = lexeme;
                } else {
                    throw std::invalid_argument(
                        "In the file '" + filePath_ +
                        "' there is an unexpected element '" + lexeme +
                        "' on line " + std::to_string(line));
                }
                break;
            }
        }

        if (type != TokenType::NONE) {
            tokens_.push_back(Token(type, lexeme, literal, line));
        }

        fileChar = advance();
    }
}

char Scanner::peek() {
    if (file_.eof()) return 0;

    char current = 0;
    file_.unget();
    file_.get(current);

    return current;
}

char Scanner::peekNext() {
    char nextSymbol = 0;
    file_.get(nextSymbol);
    file_.unget();

    return nextSymbol;
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

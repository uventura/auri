#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <variant>

namespace Auri {
enum class TokenType {
    // Math Operators
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    // Literals
    STRING,
    NUMBER,
    IDENTIFIER,
    // Variables
    GENERIC_VAR,
    NUMERIC_VAR,
    STRING_VAR,
    BOOL_VAR,
    // Keywords
    IF,
    ELSE,
    ELSE_IF,
    FOR,
    WHILE,
    RETURN,
    TRUE,
    FALSE,
    OR,
    AND,
    ALIAS,
    AS,
    IMPORT,
    FUN,
    AR_NULL,
    // Execution Control
    SETUP,
    PRE_RUN,
    RUN,
    POST_RUN,
    // EOF
    AR_EOF,
    // NONE
    NONE
};

class Token;

using TokenLiteral = std::variant<std::string, bool, double, char>;
using TokenPair = std::pair<Token, Token>;

class Token {
   private:
    TokenType type_;
    std::string lexeme_;
    TokenLiteral literal_;
    int line_;

   public:
    Token(TokenType type, std::string lexeme, TokenLiteral literal,
          uint32_t line);

    TokenType type();
    TokenLiteral literal();
    std::string typeToString();
    std::string lexeme();
    std::string literalToStr();
    std::string line();
};
}    // namespace Auri

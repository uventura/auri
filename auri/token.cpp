#include "auri/token.hpp"

namespace Auri {
Token::Token(TokenType type, std::string lexeme, TokenLiteral literal,
             uint32_t line) {
    type_ = type;
    lexeme_ = lexeme;
    literal_ = literal;
    line_ = line;
}

std::string Token::typeToString() {
    switch (type_) {
        case TokenType::LEFT_PAREN:
            return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:
            return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE:
            return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE:
            return "RIGHT_BRACE";
        case TokenType::COMMA:
            return "COMMA";
        case TokenType::DOT:
            return "DOT";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::SEMICOLON:
            return "SEMICOLON";
        case TokenType::SLASH:
            return "SLASH";
        case TokenType::STAR:
            return "STAR";
        case TokenType::BANG:
            return "BANG";
        case TokenType::BANG_EQUAL:
            return "BANG_EQUAL";
        case TokenType::EQUAL:
            return "EQUAL";
        case TokenType::EQUAL_EQUAL:
            return "EQUAL_EQUAL";
        case TokenType::GREATER:
            return "GREATER";
        case TokenType::GREATER_EQUAL:
            return "GREATER_EQUAL";
        case TokenType::LESS:
            return "LESS";
        case TokenType::LESS_EQUAL:
            return "LESS_EQUAL";
        case TokenType::STRING:
            return "STRING";
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::GENERIC_VAR:
            return "GENERIC_VAR";
        case TokenType::NUMERIC_VAR:
            return "NUMERIC_VAR";
        case TokenType::STRING_VAR:
            return "STRING_VAR";
        case TokenType::BOOL_VAR:
            return "BOOL_VAR";
        case TokenType::AR_NULL:
            return "NULL";
        case TokenType::IF:
            return "IF";
        case TokenType::ELSE:
            return "ELSE";
        case TokenType::ELSE_IF:
            return "ELSE_IF";
        case TokenType::FOR:
            return "FOR";
        case TokenType::WHILE:
            return "WHILE";
        case TokenType::RETURN:
            return "RETURN";
        case TokenType::TRUE:
            return "TRUE";
        case TokenType::FALSE:
            return "FALSE";
        case TokenType::OR:
            return "OR";
        case TokenType::AND:
            return "AND";
        case TokenType::ALIAS:
            return "ALIAS";
        case TokenType::AS:
            return "AS";
        case TokenType::IMPORT:
            return "IMPORT";
        case TokenType::FUN:
            return "FUN";
        case TokenType::SETUP:
            return "SETUP";
        case TokenType::PRE_RUN:
            return "PRE_RUN";
        case TokenType::RUN:
            return "RUN";
        case TokenType::POST_RUN:
            return "POST_RUN";
        case TokenType::AR_EOF:
            return "AR_EOF";
        case TokenType::NONE:
            return "NONE";
    }

    return "<<NOT_DEFINED>>";
}

std::string Token::lexeme() { return lexeme_; }

std::string Token::literal() {
    if (auto pval = std::get_if<std::string>(&literal_)) {
        return *pval;
    }
    if (auto pval = std::get_if<double>(&literal_)) {
        double val = *pval;
        return std::to_string(val);
    }

    return "";
}

std::string Token::line() { return std::to_string(line_); }
}    // namespace Auri

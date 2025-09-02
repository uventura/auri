#include "auri/parser.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>

namespace Auri {
namespace AST {
Parser::Parser(std::vector<Token>& tokens) : tokens_(tokens) {
    expr_ = expression();
}

ExpressionPtr Parser::expression() { return equality(); }

ExpressionPtr Parser::equality() {
    ExpressionPtr left = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token oper = previous();
        left =
            std::make_unique<BinaryExpr>(std::move(left), oper, comparison());
    }

    return left;
}

ExpressionPtr Parser::comparison() {
    ExpressionPtr left = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                  TokenType::LESS_EQUAL})) {
        Token oper = previous();
        left = std::make_unique<BinaryExpr>(std::move(left), oper, term());
    }

    return left;
}

ExpressionPtr Parser::term() {
    ExpressionPtr left = factor();

    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token oper = previous();
        left = std::make_unique<BinaryExpr>(std::move(left), oper, factor());
    }

    return left;
}

ExpressionPtr Parser::factor() {
    ExpressionPtr left = unary();

    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token oper = previous();
        left = std::make_unique<BinaryExpr>(std::move(left), oper, unary());
    }

    return left;
}

ExpressionPtr Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        return std::make_unique<UnaryExpr>(op, unary());
    }
    return primary();
}

ExpressionPtr Parser::primary() {
    switch (peek().type()) {
        case TokenType::TRUE:
        case TokenType::FALSE:
        case TokenType::AR_NULL:
        case TokenType::NUMBER:
        case TokenType::STRING:
            return std::make_unique<LiteralExpr>(peek().literal());
        default:
            break;
    }

    return NULL;
}

Token Parser::peek() { return tokens_[currentPos_]; }
Token Parser::previous() {
    if (currentPos_ - 1 <= tokens_.size() - 1) {
        throw std::runtime_error("No previous token");
    }
    return tokens_[currentPos_ - 1];
}
Token Parser::advance() { return tokens_[currentPos_++]; }
bool Parser::match(std::vector<TokenType> possibleMatches) {
    if (std::find(possibleMatches.begin(), possibleMatches.end(),
                  peek().type()) != possibleMatches.end()) {
        advance();
        return true;
    }

    return false;
}

}    // namespace AST
}    // namespace Auri
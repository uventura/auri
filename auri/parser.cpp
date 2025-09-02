#include "auri/parser.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <iostream>

namespace Auri {
namespace AST {
Parser::Parser(const std::vector<Token>& tokens) : tokens_(tokens) {
    expr_ = expression();
}

Expression& Parser::ast() {
    return *expr_;
}

ExpressionPtr Parser::expression() { return equality(); }

ExpressionPtr Parser::equality() {
    std::cout << "Equality\n";
    ExpressionPtr left = comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token oper = previous();
        left =
            std::make_unique<BinaryExpr>(std::move(left), oper, comparison());
    }

    return left;
}

ExpressionPtr Parser::comparison() {
    std::cout << "Comparison\n";
    ExpressionPtr left = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                  TokenType::LESS_EQUAL})) {
        Token oper = previous();
        left = std::make_unique<BinaryExpr>(std::move(left), oper, term());
    }

    return left;
}

ExpressionPtr Parser::term() {
    std::cout << "Term\n";
    ExpressionPtr left = factor();
    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token oper = previous();
        left = std::make_unique<BinaryExpr>(std::move(left), oper, factor());
    }

    return left;
}

ExpressionPtr Parser::factor() {
    std::cout << "Factor\n";
    ExpressionPtr left = unary();
    while (match({TokenType::SLASH, TokenType::STAR})) {
        std::cout << "Match slash and star\n";
        Token oper = previous();
        left = std::make_unique<BinaryExpr>(std::move(left), oper, unary());
    }

    return left;
}

ExpressionPtr Parser::unary() {
    std::cout << "Unary\n";
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        return std::make_unique<UnaryExpr>(op, unary());
    }
    return primary();
}

ExpressionPtr Parser::primary() {
    std::cout << "Primary " << peek().literalToStr() << "\n";
    if(match({TokenType::TRUE})) {
        return std::make_unique<LiteralExpr>(true);
    } else if(match({TokenType::FALSE})) {
        return std::make_unique<LiteralExpr>(false);
    } else if(match({TokenType::AR_NULL, TokenType::NUMBER, TokenType::STRING})) {
        return std::make_unique<LiteralExpr>(peek().literal());
    } else if(match({TokenType::AR_EOF})) {
        return nullptr;
    }
    std::cout << ">>>>" << currentPos_ << "\n";
    if(match({TokenType::LEFT_PAREN})) {
        std::cout << "done\n";
        ExpressionPtr expr = expression();
        consume(TokenType::RIGHT_PAREN, "Missing right parenthesized expression.");

        return expr;
    }

    return nullptr;
}

Token Parser::peek() { return tokens_[currentPos_]; }

Token Parser::previous() {
    if (currentPos_ < 0) {
        throw std::runtime_error("No previous token");
    }
    return tokens_[currentPos_ - 1];
}

Token Parser::advance() {
    if(tokens_.size() <= currentPos_) {
        std::runtime_error("Can't advance to next token");
    }

    return tokens_[currentPos_++];
}

void Parser::consume(TokenType expectedToken, std::string errorMessage) {
    std::cout << currentPos_ << "\n";
    if (peek().type() != expectedToken) {
        throw std::runtime_error(errorMessage);
    }

    advance();
}

bool Parser::match(std::vector<TokenType> possibleMatches) {
    std::cout << tokens_.size() << " " << currentPos_ << "\n";
    if(tokens_.size() - 1 < currentPos_) return false;

    if (std::find(possibleMatches.begin(), possibleMatches.end(),
                  peek().type()) != possibleMatches.end()) {
        advance();
        return true;
    }

    return false;
}

}    // namespace AST
}    // namespace Auri
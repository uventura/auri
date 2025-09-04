#include "auri/parser.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

namespace Auri {
namespace AST {
Parser::Parser(const std::vector<Token>& tokens) : tokens_(tokens) { parse(); }

std::vector<StatementPtr>& Parser::ast() { return program_; }

void Parser::parse() {
    while (!isAtEnd()) {
        StatementPtr statement = nullptr;

        if (match({TokenType::RUN})) {
            statement = runStmt();
        } else {
            statement = defaultStmt();
        }

        program_.push_back(std::move(statement));
    }
}

StatementPtr Parser::defaultStmt() {
    if (match({TokenType::IMPORT})) {
        return importStmt();
    } else if(match({TokenType::IF})) {
        return ifStmt();
    }

    return expressionStmt();
}

StatementPtr Parser::runStmt() {
    Token identifier = consume(TokenType::IDENTIFIER,
                               "The run statement expects an identifier");
    consume(TokenType::LEFT_BRACE, "The run statement expects a left brace");

    std::vector<StatementPtr> runStatements{};
    while (!match({TokenType::RIGHT_BRACE, TokenType::AR_EOF})) {
        runStatements.push_back(defaultStmt());
    }

    if (previous().type() == TokenType::AR_EOF) {
        consume(TokenType::RIGHT_BRACE,
                "The run statement expects a left brace");
    }

    return std::make_unique<RunStmt>(identifier, std::move(runStatements));
}

StatementPtr Parser::expressionStmt() {
    auto expr = std::make_unique<ExprStmt>(expression());
    consume({TokenType::SEMICOLON}, "Missing semicolon in expression");

    return expr;
}

StatementPtr Parser::importStmt() {
    consume(TokenType::LEFT_PAREN, "Import statement expects '('");
    Token importModule =
        consume(TokenType::STRING, "Expected string in import expression");

    std::vector<Token> moduleBlocks;
    if (match({TokenType::COMMA})) {
        while (!match({TokenType::RIGHT_PAREN, TokenType::AR_EOF})) {
            auto block =
                consume(TokenType::STRING, "Import blocks expects strings");
            moduleBlocks.push_back(block);

            if(peek().type() != TokenType::RIGHT_PAREN) {
                consume(TokenType::COMMA,
                        "Import blocks expects commas between strings");
            }
        }

        if (previous().type() == TokenType::AR_EOF) {
            throw std::runtime_error("Import module missing ')'");
        }
    } else {
        consume(TokenType::RIGHT_PAREN, "Import statement expects ')'");
    }

    consume(TokenType::SEMICOLON, "Import statements expects semicolons");
    return std::make_unique<ImportStmt>(importModule, moduleBlocks);
}

StatementPtr Parser::ifStmt() {
    consume(TokenType::LEFT_PAREN, "If statement expects '('");
    ExpressionPtr condition = expression();
    consume(TokenType::RIGHT_PAREN, "If statement expects ')'");

    std::vector<StatementPtr> thenBranch;
    if (match({TokenType::LEFT_BRACE})) {
        while (!match({TokenType::RIGHT_BRACE, TokenType::AR_EOF})) {
            thenBranch.push_back(defaultStmt());
        }

        if (previous().type() == TokenType::AR_EOF) {
            throw std::runtime_error("If statement missing '}'");
        }
    } else {
        thenBranch.push_back(defaultStmt());
    }

    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch));
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
    if (match({TokenType::TRUE})) {
        return std::make_unique<LiteralExpr>(true);
    } else if (match({TokenType::FALSE})) {
        return std::make_unique<LiteralExpr>(false);
    } else if (match({TokenType::AR_NULL, TokenType::NUMBER,
                      TokenType::STRING})) {
        return std::make_unique<LiteralExpr>(previous().literal());
    } else if (match({TokenType::AR_EOF})) {
        return nullptr;
    }
    if (match({TokenType::LEFT_PAREN})) {
        ExpressionPtr expr = expression();
        consume(TokenType::RIGHT_PAREN,
                "Missing right parenthesized expression -> Error in: '" + peek().lexeme() + "' at line [" + peek().line() + "]");

        return expr;
    }

    throw std::runtime_error("Unrecognized expression -> Error in: '" + peek().lexeme() + "' at line [" + peek().line() + "]");
}

Token Parser::peek() { return tokens_[currentPos_]; }

Token Parser::previous() {
    if (currentPos_ == 0) {
        throw std::runtime_error("No previous token");
    }
    return tokens_[currentPos_ - 1];
}

Token Parser::advance() {
    if (tokens_.size() <= currentPos_) {
        std::runtime_error("Can't advance to next token");
    }

    return tokens_[currentPos_++];
}

bool Parser::isAtEnd() { return (bool)(peek().type() == TokenType::AR_EOF); }

Token Parser::consume(TokenType expectedToken, std::string errorMessage) {
    if (peek().type() != expectedToken) {
        throw std::runtime_error(errorMessage + " -> Error in: '" + peek().lexeme() + "' at line [" + peek().line() + "]");
    }

    return advance();
}

bool Parser::match(std::vector<TokenType> possibleMatches) {
    if (tokens_.size() - 1 < currentPos_) return false;

    if (std::find(possibleMatches.begin(), possibleMatches.end(),
                  peek().type()) != possibleMatches.end()) {
        advance();
        return true;
    }

    return false;
}

}    // namespace AST
}    // namespace Auri

#pragma once

#include "auri/expression.hpp"
#include "auri/statement.hpp"
#include "auri/token.hpp"
#include <memory>
#include <vector>

namespace Auri {
namespace AST {
class Parser {
   private:
    std::vector<StatementPtr> program_;
    uint64_t currentPos_ = 0;
    std::vector<Token>
        tokens_;    // No need to keep this data after the tree generated.

    void parse();

    StatementPtr statement();
    StatementPtr defaultStmt();
    StatementPtr runStmt();
    StatementPtr expressionStmt();
    StatementPtr importStmt();
    StatementPtr ifStmt();
    std::vector<StatementPtr> blockStmt();

    ExpressionPtr expression();
    ExpressionPtr equality();
    ExpressionPtr comparison();
    ExpressionPtr term();
    ExpressionPtr factor();
    ExpressionPtr unary();
    ExpressionPtr primary();

    Token peek();
    Token previous();
    Token advance();
    Token consume(TokenType expectedToken, std::string errorMessage);

    bool isAtEnd();
    bool match(std::vector<TokenType> possibleMatches);

   public:
    Parser(const std::vector<Token>& tokens);

    std::vector<StatementPtr>& ast();
};
}    // namespace AST
}    // namespace Auri

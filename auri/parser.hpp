#pragma once

#include "auri/expression.hpp"
#include "auri/token.hpp"
#include <memory>
#include <vector>

namespace Auri {
namespace AST {
class Parser {
   private:
    ExpressionPtr expr_;
    uint64_t currentPos_ = 0;
    std::vector<Token>&
        tokens_;    // No need to keep this data after the tree generated.

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
    bool match(std::vector<TokenType> possibleMatches);

   public:
    Parser(std::vector<Token>& tokens);
};
}    // namespace AST
}    // namespace Auri
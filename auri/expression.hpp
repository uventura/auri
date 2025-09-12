#pragma once
#include "auri/token.hpp"
#include <memory>
#include <vector>

namespace Auri {
namespace AST {
class Expression;
class LiteralExpr;
class GroupingExpr;
class UnaryExpr;
class BinaryExpr;
class VariableExpr;
class CallExpr;
using ExpressionPtr = std::unique_ptr<Expression>;

class ExpressionVisitor {
   public:
    virtual ExpressionPtr visit(LiteralExpr& expr) = 0;
    virtual ExpressionPtr visit(GroupingExpr& expr) = 0;
    virtual ExpressionPtr visit(UnaryExpr& expr) = 0;
    virtual ExpressionPtr visit(BinaryExpr& expr) = 0;
    virtual ExpressionPtr visit(VariableExpr& expr) = 0;
    virtual ExpressionPtr visit(CallExpr& expr) = 0;
    virtual ~ExpressionVisitor() = default;
};

class Expression {
   public:
    virtual ExpressionPtr accept(ExpressionVisitor& visitor) = 0;
    virtual ~Expression() = default;
};

class LiteralExpr : public Expression {
   private:
    TokenLiteral literal_;

   public:
    LiteralExpr(TokenLiteral literal) : literal_(literal) {}
    ExpressionPtr accept(ExpressionVisitor& visitor) {
        return visitor.visit(*this);
    };
    TokenLiteral literal() { return literal_; };
};

class GroupingExpr : public Expression {
   private:
    ExpressionPtr expr_;

   public:
    GroupingExpr(ExpressionPtr expr) : expr_(std::move(expr)) {}
    ExpressionPtr accept(ExpressionVisitor& visitor) {
        return visitor.visit(*this);
    };
    Expression& expr() { return *expr_.get(); };
};

class UnaryExpr : public Expression {
   private:
    Token op_;
    ExpressionPtr term_;

   public:
    UnaryExpr(Token op, ExpressionPtr term) : op_(op), term_(std::move(term)) {}
    ExpressionPtr accept(ExpressionVisitor& visitor) {
        return visitor.visit(*this);
    };
    Token op() { return op_; };
    Expression& term() { return *term_.get(); };
};

class BinaryExpr : public Expression {
   private:
    ExpressionPtr left_;
    Token op_;
    ExpressionPtr right_;

   public:
    BinaryExpr(ExpressionPtr left, Token op, ExpressionPtr right)
        : left_(std::move(left)), op_(op), right_(std::move(right)) {}
    ExpressionPtr accept(ExpressionVisitor& visitor) {
        return visitor.visit(*this);
    };
    Expression& left() { return *left_.get(); };
    Token op() { return op_; };
    Expression& right() { return *right_.get(); };
};

class VariableExpr : public Expression {
   private:
    Token name_;

   public:
    VariableExpr(Token name) : name_(name) {}
    ExpressionPtr accept(ExpressionVisitor& visitor) {
        return visitor.visit(*this);
    };
    Token name() { return name_; };
};

class CallExpr : public Expression {
   private:
    Token name_;
    std::vector<ExpressionPtr> arguments_;

   public:
    CallExpr(Token name, std::vector<ExpressionPtr> arguments)
        : name_(name), arguments_(std::move(arguments)) {}
    ExpressionPtr accept(ExpressionVisitor& visitor) {
        return visitor.visit(*this);
    };
    Token name() { return name_; };
    std::vector<ExpressionPtr>& arguments() { return arguments_; };
};

}    // namespace AST
}    // namespace Auri

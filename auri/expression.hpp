#pragma once
#include "auri/token.hpp"
#include <memory>

namespace Auri {
namespace AST {
class Expression;
class LiteralExpr;
class GroupingExpr;
class UnaryExpr;
class BinaryExpr;
class VariableExpr;
using ExpressionPtr = std::unique_ptr<Expression>;

class ExpressionVisitor {
   public:
    virtual void visit(LiteralExpr& expr) = 0;
    virtual void visit(GroupingExpr& expr) = 0;
    virtual void visit(UnaryExpr& expr) = 0;
    virtual void visit(BinaryExpr& expr) = 0;
    virtual void visit(VariableExpr& expr) = 0;
    virtual ~ExpressionVisitor() = default;
};

class Expression {
   public:
    virtual void accept(ExpressionVisitor& visitor) = 0;
    virtual ~Expression() = default;
};

class LiteralExpr : public Expression {
   private:
    TokenLiteral literal_;

   public:
    LiteralExpr(TokenLiteral literal) : literal_(literal) {}
    void accept(ExpressionVisitor& visitor) { return visitor.visit(*this); };
    TokenLiteral literal() { return literal_; };
};

class GroupingExpr : public Expression {
   private:
    ExpressionPtr expr_;

   public:
    GroupingExpr(ExpressionPtr expr) : expr_(std::move(expr)) {}
    void accept(ExpressionVisitor& visitor) { return visitor.visit(*this); };
    Expression& expr() { return *expr_.get(); };
};

class UnaryExpr : public Expression {
   private:
    Token op_;
    ExpressionPtr term_;

   public:
    UnaryExpr(Token op, ExpressionPtr term) : op_(op), term_(std::move(term)) {}
    void accept(ExpressionVisitor& visitor) { return visitor.visit(*this); };
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
    void accept(ExpressionVisitor& visitor) { return visitor.visit(*this); };
    Expression& left() { return *left_.get(); };
    Token op() { return op_; };
    Expression& right() { return *right_.get(); };
};

class VariableExpr : public Expression {
   private:
    Token name_;

   public:
    VariableExpr(Token name) : name_(name) {}
    void accept(ExpressionVisitor& visitor) { return visitor.visit(*this); };
    Token name() { return name_; };
};

}    // namespace AST
}    // namespace Auri

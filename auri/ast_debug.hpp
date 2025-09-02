#pragma once

#include "auri/expression.hpp"

namespace Auri {
namespace AST {
class AstDebug : public ExpressionVisitor {
   private:
    Expression& expr_;
    int spaceCounter_ = 0;

    void spacement();

   public:
    AstDebug(Expression& expr);
    void print();

    void visit(LiteralExpr& expr);
    void visit(GroupingExpr& expr);
    void visit(UnaryExpr& expr);
    void visit(BinaryExpr& expr);
};
}    // namespace AST
}    // namespace Auri
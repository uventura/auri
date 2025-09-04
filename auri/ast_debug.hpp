#pragma once

#include "auri/expression.hpp"
#include "auri/statement.hpp"

namespace Auri {
namespace AST {
class AstDebug : public ExpressionVisitor, public StatementVisitor {
   private:
    std::vector<StatementPtr>& program_;
    int spaceCounter_ = 0;

    void spacement();

   public:
    AstDebug(std::vector<StatementPtr>& program);
    void print();

    // Statement visitors
    void visit(ExprStmt& expr);
    void visit(RunStmt& run);
    void visit(ImportStmt& import);
    void visit(IfStmt& ifs);

    // Expression vistors
    void visit(LiteralExpr& expr);
    void visit(GroupingExpr& expr);
    void visit(UnaryExpr& expr);
    void visit(BinaryExpr& expr);
};
}    // namespace AST
}    // namespace Auri

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
    StatementPtr visit(ExprStmt& expr);
    StatementPtr visit(RunStmt& run);
    StatementPtr visit(ImportStmt& import);
    StatementPtr visit(IfStmt& ifs);
    StatementPtr visit(WhileStmt& whiles);
    StatementPtr visit(VarStmt& stmt);
    StatementPtr visit(FunctionStmt& stmt);
    StatementPtr visit(ReturnStmt& stmt);

    // Expression vistors
    ExpressionPtr visit(LiteralExpr& expr);
    ExpressionPtr visit(GroupingExpr& expr);
    ExpressionPtr visit(UnaryExpr& expr);
    ExpressionPtr visit(BinaryExpr& expr);
    ExpressionPtr visit(VariableExpr& expr);
    ExpressionPtr visit(CallExpr& expr);
};
}    // namespace AST
}    // namespace Auri

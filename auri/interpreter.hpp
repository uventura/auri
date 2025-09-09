#pragma once

#include "auri/expression.hpp"
#include "auri/statement.hpp"

namespace Auri {
namespace AST {
class Interpreter : public ExpressionVisitor, public StatementVisitor {
   private:
    std::vector<StatementPtr>& program_;

   public:
    Interpreter(std::vector<StatementPtr>& program);
    void run();

    // Statement visitors
    void visit(ExprStmt& expr);
    void visit(RunStmt& run);
    void visit(ImportStmt& import);
    void visit(IfStmt& ifs);
    void visit(WhileStmt& whiles);
    void visit(VarStmt& stmt);
    void visit(FunctionStmt& stmt);
    void visit(ReturnStmt& stmt);

    // Expression vistors
    void visit(LiteralExpr& expr);
    void visit(GroupingExpr& expr);
    void visit(UnaryExpr& expr);
    void visit(BinaryExpr& expr);
    void visit(VariableExpr& expr);
    void visit(CallExpr& expr);
};
}    // namespace AST
}    // namespace Auri

#pragma once

#include "auri/expression.hpp"
#include <memory>
#include <vector>

namespace Auri {
namespace AST {
class Statement;
class ExprStmt;
class RunStmt;
using StatementPtr = std::unique_ptr<Statement>;

class StatementVisitor {
   public:
    virtual void visit(ExprStmt& stmt) = 0;
    virtual void visit(RunStmt& stmt) = 0;
    virtual ~StatementVisitor() = default;
};

class Statement {
   public:
    virtual void accept(StatementVisitor& statement) = 0;
    virtual ~Statement() = default;
};

class ExprStmt : public Statement {
   private:
    ExpressionPtr expr_;

   public:
    ExprStmt(ExpressionPtr expr) : expr_(std::move(expr)){};
    void accept(StatementVisitor& statement) { statement.visit(*this); };

    Expression& expr() { return *expr_; };
};

class RunStmt : public Statement {
   private:
    std::vector<StatementPtr> stmt_;

   public:
    RunStmt(std::vector<StatementPtr> stmt) : stmt_(std::move(stmt)){};
    void accept(StatementVisitor& statement) { statement.visit(*this); };

    std::vector<StatementPtr>& stmt() { return stmt_; };
};
}    // namespace AST
}    // namespace Auri
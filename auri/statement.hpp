#pragma once

#include "auri/expression.hpp"
#include <memory>
#include <vector>

namespace Auri {
namespace AST {
class Statement;
class ExprStmt;
class RunStmt;
class ImportStmt;
using StatementPtr = std::unique_ptr<Statement>;

class StatementVisitor {
   public:
    virtual void visit(ExprStmt& stmt) = 0;
    virtual void visit(RunStmt& stmt) = 0;
    virtual void visit(ImportStmt& stmt) = 0;
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
    Token identifier_;
    std::vector<StatementPtr> stmt_;

   public:
    RunStmt(Token identifier, std::vector<StatementPtr> stmt)
        : identifier_(identifier), stmt_(std::move(stmt)){};
    void accept(StatementVisitor& statement) { statement.visit(*this); };

    Token identifier() { return identifier_; }
    std::vector<StatementPtr>& stmt() { return stmt_; };
};

class ImportStmt : public Statement {
   private:
    Token importedModule_;
    std::vector<Token> moduleBlocks_;

   public:
    ImportStmt(Token importedModule, std::vector<Token> moduleBlocks)
        : importedModule_(importedModule),
          moduleBlocks_(std::move(moduleBlocks)){};
    void accept(StatementVisitor& statement) { statement.visit(*this); };

    Token importedModule() { return importedModule_; }
    const std::vector<Token>& moduleBlocks() { return moduleBlocks_; }
};
}    // namespace AST
}    // namespace Auri
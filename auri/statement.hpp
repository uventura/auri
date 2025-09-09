#pragma once

#include "auri/expression.hpp"
#include <memory>
#include <utility>
#include <vector>

namespace Auri {
namespace AST {
class Statement;
class ExprStmt;
class RunStmt;
class ImportStmt;
class IfStmt;
class WhileStmt;
class VarStmt;
class FunctionStmt;
class ReturnStmt;
using StatementPtr = std::unique_ptr<Statement>;

class StatementVisitor {
   public:
    virtual StatementPtr visit(ExprStmt& stmt) = 0;
    virtual StatementPtr visit(RunStmt& stmt) = 0;
    virtual StatementPtr visit(ImportStmt& stmt) = 0;
    virtual StatementPtr visit(IfStmt& stmt) = 0;
    virtual StatementPtr visit(WhileStmt& stmt) = 0;
    virtual StatementPtr visit(VarStmt& stmt) = 0;
    virtual StatementPtr visit(FunctionStmt& stmt) = 0;
    virtual StatementPtr visit(ReturnStmt& stmt) = 0;
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

class IfStmt : public Statement {
   private:
    ExpressionPtr condition_;
    std::vector<StatementPtr> thenBranch_;

   public:
    IfStmt(ExpressionPtr condition, std::vector<StatementPtr> thenBranch)
        : condition_(std::move(condition)),
          thenBranch_(std::move(thenBranch)){};
    void accept(StatementVisitor& statement) { statement.visit(*this); };

    Expression& condition() { return *condition_; }
    std::vector<StatementPtr>& thenBranch() { return thenBranch_; }
};

class WhileStmt : public Statement {
   private:
    ExpressionPtr condition_;
    std::vector<StatementPtr> body_;

   public:
    WhileStmt(ExpressionPtr condition, std::vector<StatementPtr> body)
        : condition_(std::move(condition)), body_(std::move(body)){};
    void accept(StatementVisitor& statement) { statement.visit(*this); };

    Expression& condition() { return *condition_; }
    std::vector<StatementPtr>& body() { return body_; }
};

class VarStmt : public Statement {
   private:
    Token identifier_;
    ExpressionPtr initializer_;    // Can be null

   public:
    VarStmt(Token identifier, ExpressionPtr initializer)
        : identifier_(identifier), initializer_(std::move(initializer)){};
    void accept(StatementVisitor& statement) { statement.visit(*this); };

    Token identifier() { return identifier_; }
    Expression& initializer() { return *initializer_; }
};

class FunctionStmt : public Statement {
   private:
    Token name_;
    std::vector<TokenPair> params_;
    std::vector<StatementPtr> body_;

   public:
    FunctionStmt(Token name, std::vector<TokenPair> params,
                 std::vector<StatementPtr> body)
        : name_(name), params_(std::move(params)), body_(std::move(body)){};
    void accept(StatementVisitor& statement) { statement.visit(*this); };

    Token name() { return name_; }
    const std::vector<TokenPair>& params() { return params_; }
    std::vector<StatementPtr>& body() { return body_; }
};

class ReturnStmt : public Statement {
   private:
    ExpressionPtr expr_;

   public:
    ReturnStmt(ExpressionPtr expr) : expr_(std::move(expr)){};
    void accept(StatementVisitor& statement) { statement.visit(*this); };

    Expression& expr() { return *expr_; }
};
}    // namespace AST
}    // namespace Auri

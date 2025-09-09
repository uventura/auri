#include "auri/interpreter.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <variant>

namespace Auri {
namespace AST {
Interpreter::Interpreter(std::vector<StatementPtr>& program)
    : program_(program) {}

void Interpreter::run() {
    for (uint64_t i = 0; i < program_.size(); ++i) {
        std::cout << "\n--------------------------\n";
        (*program_[i]).accept(*this);
    }
}

StatementPtr Interpreter::visit(ExprStmt& expr) { return nullptr; }
StatementPtr Interpreter::visit(RunStmt& run) { return nullptr; }
StatementPtr Interpreter::visit(ImportStmt& import) { return nullptr; }
StatementPtr Interpreter::visit(IfStmt& ifs) { return nullptr; }
StatementPtr Interpreter::visit(WhileStmt& whiles) { return nullptr; }
StatementPtr Interpreter::visit(VarStmt& stmt) { return nullptr; }
StatementPtr Interpreter::visit(FunctionStmt& stmt) { return nullptr; }
StatementPtr Interpreter::visit(ReturnStmt& stmt) { return nullptr; }
ExpressionPtr Interpreter::visit(LiteralExpr& expr) { return nullptr; }
ExpressionPtr Interpreter::visit(GroupingExpr& expr) { return nullptr; }
ExpressionPtr Interpreter::visit(UnaryExpr& expr) { return nullptr; }
ExpressionPtr Interpreter::visit(BinaryExpr& expr) { return nullptr; }
ExpressionPtr Interpreter::visit(VariableExpr& expr) { return nullptr; }
ExpressionPtr Interpreter::visit(CallExpr& expr) { return nullptr; }

}    // namespace AST
}    // namespace Auri

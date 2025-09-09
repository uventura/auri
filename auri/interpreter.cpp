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

void Interpreter::visit(ExprStmt& expr) {}
void Interpreter::visit(RunStmt& run) {}
void Interpreter::visit(ImportStmt& import) {}
void Interpreter::visit(IfStmt& ifs) {}
void Interpreter::visit(WhileStmt& whiles) {}
void Interpreter::visit(VarStmt& stmt) {}
void Interpreter::visit(FunctionStmt& stmt) {}
void Interpreter::visit(ReturnStmt& stmt) {}
void Interpreter::visit(LiteralExpr& expr) {}
void Interpreter::visit(GroupingExpr& expr) {}
void Interpreter::visit(UnaryExpr& expr) {}
void Interpreter::visit(BinaryExpr& expr) {}
void Interpreter::visit(VariableExpr& expr) {}
void Interpreter::visit(CallExpr& expr) {}

}    // namespace AST
}    // namespace Auri

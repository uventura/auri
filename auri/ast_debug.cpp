#include "auri/ast_debug.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <variant>

namespace Auri {
namespace AST {
AstDebug::AstDebug(Expression& expr) : expr_(expr) {}

void AstDebug::print() {
    std::cout << "+===============================+\n";
    std::cout << "|            Auri AST           |\n";
    std::cout << "+===============================+\n";

    expr_.accept(*this);

    std::cout << "+===============================+\n";
    std::cout << "|             End AST           |\n";
    std::cout << "+===============================+\n";
}

void AstDebug::visit(LiteralExpr& expr) {
    spacement();
    spaceCounter_++;

    auto print = [](auto&& value) { std::cout << value << "\n"; };
    auto literal = expr.literal();

    std::visit(print, literal);
    spaceCounter_--;
}

void AstDebug::visit(GroupingExpr& expr) {
    spaceCounter_++;
    expr.expr().accept(*this);
    spaceCounter_--;
}

void AstDebug::visit(UnaryExpr& expr) {
    spacement();
    std::cout << "[Unary] <<\n";

    spaceCounter_++;
    spacement();
    std::cout << " \\\n";
    spacement();
    std::cout << " [" << expr.op().lexeme() << "]\n";
    spacement();
    std::cout << " /\n";
    spaceCounter_--;

    spaceCounter_++;
    expr.term().accept(*this);
    spaceCounter_--;

    spacement();
    std::cout << ">>\n";
}

void AstDebug::visit(BinaryExpr& expr) {
    spacement();
    std::cout << "[Binary Expr] <<\n";

    spaceCounter_++;
    expr.left().accept(*this);
    spaceCounter_--;

    spaceCounter_++;
    spacement();
    std::cout << " \\\n";
    spacement();
    std::cout << " [" << expr.op().lexeme() << "]\n";
    spacement();
    std::cout << " /\n";
    spaceCounter_--;

    spaceCounter_++;
    expr.right().accept(*this);
    spaceCounter_--;

    spacement();
    std::cout << ">>\n";
}

void AstDebug::spacement() {
    for (int i = 0; i < spaceCounter_; ++i) {
        std::cout << "|   ";
    }
}
}    // namespace AST
}    // namespace Auri

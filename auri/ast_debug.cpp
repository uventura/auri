#include "auri/ast_debug.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <variant>

namespace Auri {
namespace AST {
AstDebug::AstDebug(Expression& expr) : expr_(expr) {}

void AstDebug::print() { expr_.accept(*this); }

void AstDebug::visit(LiteralExpr& expr) {
    // auto literal = expr.literal();
    // if (auto pval = std::get_if<std::string>(literal)) {
    //     std::cout << *pval << "\n";
    // }
    // if (auto pval = std::get_if<double>(literal)) {
    //     double val = *pval;
    //     std::cout << std::to_string(val) << "\n";
    // }

    // std::cout << "\n";
    std::cout << "(K)\n";
}

void AstDebug::visit(GroupingExpr& expr) {
    std::cout << "Grouping<< ";
    expr.expr().accept(*this);
    std::cout << " >>\n";
}

void AstDebug::visit(UnaryExpr& expr) {
    std::cout << "Unary<< ";
    std::cout << expr.op().lexeme() << " ";
    expr.term();
    std::cout << " >>\n";
}

void AstDebug::visit(BinaryExpr& expr) {
    std::cout << "Binary<< ";
    expr.left();
    std::cout << expr.op().lexeme() << "\n";
    expr.right();
    std::cout << ">>\n";
}
}    // namespace AST
}    // namespace Auri
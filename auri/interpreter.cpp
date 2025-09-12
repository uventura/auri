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
        (*program_[i]).accept(*this);
    }
}

StatementPtr Interpreter::visit(ExprStmt& expr) {
    expr.expr().accept(*this);

    return nullptr;
 }
StatementPtr Interpreter::visit(RunStmt& run) {
    std::cout << "----[ Run: " << run.identifier().literalToStr() << " ]----\n\n";
    for(uint64_t i=0; i < run.stmt().size(); ++i) {
        (*run.stmt()[i]).accept(*this);
    }
    std::cout << "\n----[ End: " << run.identifier().literalToStr() << " ]----\n";

    return nullptr;
}
StatementPtr Interpreter::visit(ImportStmt& import) { return nullptr; }
StatementPtr Interpreter::visit(IfStmt& ifs) { return nullptr; }
StatementPtr Interpreter::visit(WhileStmt& whiles) { return nullptr; }
StatementPtr Interpreter::visit(VarStmt& stmt) { return nullptr; }
StatementPtr Interpreter::visit(FunctionStmt& stmt) { return nullptr; }
StatementPtr Interpreter::visit(ReturnStmt& stmt) { return nullptr; }

ExpressionPtr Interpreter::visit(LiteralExpr& expr) {
    return std::make_unique<LiteralExpr>(expr.literal());
}

ExpressionPtr Interpreter::visit(GroupingExpr& expr) { return nullptr; }
ExpressionPtr Interpreter::visit(UnaryExpr& expr) {
    TokenLiteral result = castLiteral(expr.term().accept(*this));

    if (expr.op().type() == TokenType::MINUS) {
        if (std::holds_alternative<double>(result)) {
            result = -std::get<double>(result);
        } else {
            throw std::runtime_error("Unary '-' operator expects a number");
        }
    } else if (expr.op().type() == TokenType::BANG) {
        if (std::holds_alternative<bool>(result)) {
            result = !std::get<bool>(result);
        } else {
            throw std::runtime_error("Unary '!' operator expects a boolean");
        }
    } else {
        throw std::runtime_error("Unknown unary operator");
    }

    return std::make_unique<LiteralExpr>(result);
}

ExpressionPtr Interpreter::visit(BinaryExpr& expr) {
    auto getNumber = [](TokenLiteral literal) -> double {
        if (std::holds_alternative<double>(literal)) {
            return std::get<double>(literal);
        } else {
            throw std::runtime_error("Expected a number");
        }
    };

    TokenLiteral left = castLiteral(expr.left().accept(*this));
    TokenLiteral right = castLiteral(expr.right().accept(*this));

    double leftValue = getNumber(left);
    double rightValue = getNumber(right);
    double result = 0.0;

    if(expr.op().type() == TokenType::PLUS) {
        result = leftValue + rightValue;
    } else if(expr.op().type() == TokenType::MINUS) {
        result = leftValue - rightValue;
    } else if(expr.op().type() == TokenType::STAR) {
        result = leftValue * rightValue;
    } else if(expr.op().type() == TokenType::SLASH) {
        if(rightValue == 0) {
            throw std::runtime_error("Division by zero");
        }
        result = leftValue / rightValue;
    } else {
        throw std::runtime_error("Unknown binary operator");
    }

    return std::make_unique<LiteralExpr>(result);
}

ExpressionPtr Interpreter::visit(VariableExpr& expr) { return nullptr; }
ExpressionPtr Interpreter::visit(CallExpr& expr) {
    if(expr.name().lexeme() == "print") {
        print(expr.arguments());
    }
    return nullptr;
}

TokenLiteral Interpreter::castLiteral(ExpressionPtr expr) {
    LiteralExpr* literal = dynamic_cast<LiteralExpr*>(expr.get());
    return literal->literal();
}

void Interpreter::print(std::vector<ExpressionPtr>& args) {
    for(uint32_t i = 0; i < args.size(); ++i) {
        TokenLiteral literal = castLiteral(args[i]->accept(*this));
        if (std::holds_alternative<std::string>(literal)) {
            std::cout << std::get<std::string>(literal);
        } else if (std::holds_alternative<bool>(literal)) {
            std::cout << (std::get<bool>(literal) ? "true" :
                                            "false");
        } else if (std::holds_alternative<double>(literal)) {
            std::cout << std::get<double>(literal);
        }

        if(i < args.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << "\n";
}

}    // namespace AST
}    // namespace Auri

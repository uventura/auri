#include "auri/ast_debug.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <variant>

namespace Auri {
namespace AST {
AstDebug::AstDebug(std::vector<StatementPtr>& program) : program_(program) {}

void AstDebug::print() {
    std::cout << "+===============================+\n";
    std::cout << "|            Auri AST           |\n";
    std::cout << "+===============================+\n";

    for (uint64_t i = 0; i < program_.size(); ++i) {
        std::cout << "\n--------------------------\n";
        (*program_[i]).accept(*this);
    }

    std::cout << "+===============================+\n";
    std::cout << "|             End AST           |\n";
    std::cout << "+===============================+\n";
}

void AstDebug::visit(ExprStmt& expr) {
    spacement();
    spaceCounter_++;
    std::cout << "{ExpressionStatement} <<\n";

    expr.expr().accept(*this);

    spaceCounter_--;
    spacement();
    std::cout << "{ExpressionStatement} >>\n";
}

void AstDebug::visit(RunStmt& run) {
    spacement();
    spaceCounter_++;
    std::cout << "{RunStatement} (" << run.identifier().literalToStr() << ") <<\n";

    for (uint64_t i = 0; i < run.stmt().size(); ++i) {
        spacement();
        std::cout<<"\n";
        (*(run.stmt()[i])).accept(*this);
    }

    spaceCounter_--;
    spacement();
    std::cout << "{RunStatement} >>\n";
}

void AstDebug::visit(ImportStmt& import) {
    spacement();
    spaceCounter_++;
    std::cout << "{ImportStatement} <<\n";

    spacement();
    std::cout << "- Module: " << import.importedModule().literalToStr() << "\n";

    for(auto i: import.moduleBlocks()) {
        spacement();
        std::cout << "|-- " << i.literalToStr() << "\n";
    }

    spaceCounter_--;
    spacement();
    std::cout << "{ImportStatement} >>\n";
}

void AstDebug::visit(IfStmt& ifs) {
    spacement();
    spaceCounter_++;
    std::cout << "{IfStatement} << \n";

    spacement();
    std::cout << "Condition: \n";
    ifs.condition().accept(*this);

    spacement();
    std::cout << "[Block] << \n";
    spaceCounter_++;
    for(auto& stmt : ifs.thenBranch()) {
        stmt->accept(*this);
    }
    spaceCounter_--;
    spacement();
    std::cout << "[Block] >> \n";

    spaceCounter_--;
    spacement();
    std::cout << "{IfStatement} >> \n";
}

void AstDebug::visit(WhileStmt& whiles) {
    spacement();
    spaceCounter_++;
    std::cout << "{WhileStatement} << \n";

    spacement();
    std::cout << "Condition: \n";
    whiles.condition().accept(*this);

    spacement();
    std::cout << "[Block] << \n";
    spaceCounter_++;
    for(auto& stmt : whiles.body()) {
        stmt->accept(*this);
    }
    spaceCounter_--;
    spacement();
    std::cout << "[Block] >> \n";

    spaceCounter_--;
    spacement();
    std::cout << "{WhileStatement} >> \n";
}

void AstDebug::visit(VarStmt& stmt) {
    spacement();
    spaceCounter_++;
    std::cout << "{VariableStatement} (" << stmt.identifier().literalToStr() << ") <<\n";

    if(&stmt.initializer() != nullptr) {
        spacement();
        std::cout << "Initializer: \n";
        spaceCounter_++;
        stmt.initializer().accept(*this);
        spaceCounter_--;
    } else {
        spacement();
        std::cout << "No Initializer\n";
    }

    spaceCounter_--;
    spacement();
    std::cout << "{VariableStatement} >>\n";
}

void AstDebug::visit(FunctionStmt& stmt) {
    spacement();
    spaceCounter_++;
    std::cout << "{FunctionStatement} (" << stmt.name().literalToStr() << ") <<\n";

    spacement();
    spaceCounter_++;
    std::cout << "[Parameters]:\n";
    for(auto param: stmt.params()) {
        spacement();
        Token name = param.first;
        Token type = param.second;
        std::cout << "- Name: " << name.literalToStr() << " ; Type: " << type.literalToStr() << "\n";
    }
    spaceCounter_--;

    spaceCounter_--;
    spacement();
    std::cout << "{FunctionStatement} >>\n";
}

void AstDebug::visit(ReturnStmt& stmt) {
    spacement();
    spaceCounter_++;
    std::cout << "{ReturnStatement} <<\n";

    stmt.expr().accept(*this);

    spacement();
    spaceCounter_--;
    std::cout << "{ReturnStatement} >>\n";
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

void AstDebug::visit(VariableExpr& expr) {
    spacement();
    std::cout << "[Variable] " << expr.name().literalToStr() << "\n";
}

void AstDebug::spacement() {
    for (int i = 0; i < spaceCounter_; ++i) {
        std::cout << "|   ";
    }
}
}    // namespace AST
}    // namespace Auri

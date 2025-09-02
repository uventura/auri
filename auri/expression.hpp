#include "auri/token.hpp"

#include <memory>

namespace Auri{
    class ExpressionVisitor{
        public:
			virtual void visit(LiteralExpr& expr) = 0;
			virtual void visit(GroupingExpr& expr) = 0;
			virtual void visit(UnaryExpr& expr) = 0;
			virtual void visit(BinaryExpr& expr) = 0;;
            virtual ~ExpressionVisitor() = default;
    };

    class Expression{
        public:
            virtual void accept(ExpressionVisitor& visitor) = 0;
            virtual ~Expression() = default;
    };

    class LiteralExpr: public Expression {
        private:
            TokenLiteral literal_;
        public:
            LiteralExpr(TokenLiteral literal): literal_(literal){}
            void accept(ExpressionVisitor& visitor);
    };

    class GroupingExpr: public Expression {
        private:
            std::unique_ptr<Expression> expr_;
        public:
            GroupingExpr(std::unique_ptr<Expression> expr): expr_(std::move(expr)){}
            void accept(ExpressionVisitor& visitor);
    };

    class UnaryExpr: public Expression {
        private:
            Token op_;
			std::unique_ptr<Expression> term_;
        public:
            UnaryExpr(Token op, std::unique_ptr<Expression> term): op_(op), term_(std::move(term)){}
            void accept(ExpressionVisitor& visitor);
    };

    class BinaryExpr: public Expression {
        private:
            std::unique_ptr<Expression> left_;
			Token op_;
			std::unique_ptr<Expression> right_;
        public:
            BinaryExpr(std::unique_ptr<Expression> left, Token op, std::unique_ptr<Expression> right): left_(std::move(left)), op_(op), right_(std::move(right)){}
            void accept(ExpressionVisitor& visitor);
    };

}

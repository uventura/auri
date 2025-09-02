from argparse import ArgumentParser
from string import Template

EXPRESSION_TEMPLATE = Template("""#include "auri/token.hpp"

#include <memory>

namespace Auri{
    class ExpressionVisitor{
        public:
$expression_signatures;
            virtual ~ExpressionVisitor() = default;
    };

    class Expression{
        public:
            virtual void accept(ExpressionVisitor& visitor) = 0;
            virtual ~Expression() = default;
    };
$expression_classes
}
""")

EXPRESSION_VISITOR_TEMPLATE = Template("""
    class $expr_class: public Expression {
        private:
            $attributes
        public:
            $expr_class($arguments): $initialization{}
            void accept(ExpressionVisitor& visitor);
    };
""")

def arguments():
    arg_parser = ArgumentParser()
    arg_parser.add_argument("--output", required=True)

    return arg_parser.parse_args()

def expression_visitor(args):
    grammar = {
        "LiteralExpr": ["TokenLiteral literal"],
        "GroupingExpr": ["std::unique_ptr<Expression> expr"],
        "UnaryExpr": ["Token op", "std::unique_ptr<Expression> term"],
        "BinaryExpr": ["std::unique_ptr<Expression> left", "Token op", "std::unique_ptr<Expression> right"],
    }

    classes = ""
    for expr_class, arguments in grammar.items():
        attributes = [item + "_" for item in arguments]
        initializations = []
        for index, attribute in enumerate(attributes):
            attribute_name = attribute.split()[-1]
            argument_name = arguments[index].split()[-1]
            if attribute.startswith("std::unique_ptr<Expression>"):
                initialization = f"{attribute_name}(std::move({argument_name}))"
            else:
                initialization = f"{attribute_name}({argument_name})"
            initializations.append(initialization)
        visitor_class = EXPRESSION_VISITOR_TEMPLATE.substitute(
            expr_class=expr_class,
            arguments=", ".join(arguments),
            attributes = ";\n\t\t\t".join(attributes) + ";",
            initialization=", ".join(initializations),
        )

        classes += visitor_class

    expression_signatures = [f"\t\t\tvirtual void visit({expr}& expr) = 0;" for expr in grammar]
    expression_content = EXPRESSION_TEMPLATE.substitute(
        expression_signatures="\n".join(expression_signatures),
        expression_classes = classes
    )
    with open(args.output + "/expression.hpp", "w", encoding="utf8") as expression_file:
        expression_file.write(expression_content)

def generate_visitors(args):
    expression_visitor(args)

def main():
    args = arguments()
    generate_visitors(args)

if __name__ == "__main__":
    main()
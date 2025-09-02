from argparse import ArgumentParser
from string import Template

EXPRESSION_TEMPLATE = Template("""#pragma once
#include "auri/token.hpp"

#include <memory>

namespace Auri{
namespace AST{
    class Expression;
$forward_expression_signatures
    using ExpressionPtr = std::unique_ptr<Expression>;

    class ExpressionVisitor{
        public:
$expression_signatures
            virtual ~ExpressionVisitor() = default;
    };

    class Expression{
        public:
            virtual void accept(ExpressionVisitor& visitor) = 0;
            virtual ~Expression() = default;
    };
$expression_classes
}
}
""")

EXPRESSION_VISITOR_TEMPLATE = Template("""
    class $expr_class: public Expression {
        private:
            $attributes
        public:
            $expr_class($arguments): $initialization{}
            void accept(ExpressionVisitor& visitor){return visitor.visit(*this);};
$expr_get_methods
    };
""")

def arguments():
    arg_parser = ArgumentParser()
    arg_parser.add_argument("--output", required=True)

    return arg_parser.parse_args()

def method_signature(grammar_item):
    method_type, method_name = grammar_item.split()

    prefix = ""
    postfix = ""
    if "Ptr" in method_type :
        method_type = method_type.replace("Ptr", "&")
        prefix = "*"
        postfix = ".get()"

    return method_type + " " + method_name + "() {return " + prefix + method_name + "_" + postfix + ";};\n"

def expression_visitor(args):
    grammar = {
        "LiteralExpr": ["TokenLiteral literal"],
        "GroupingExpr": ["ExpressionPtr expr"],
        "UnaryExpr": ["Token op", "ExpressionPtr term"],
        "BinaryExpr": ["ExpressionPtr left", "Token op", "ExpressionPtr right"],
    }

    classes = ""
    for expr_class, arguments in grammar.items():
        attributes = [item + "_" for item in arguments]
        initializations = []
        for index, attribute in enumerate(attributes):
            attribute_name = attribute.split()[-1]
            argument_name = arguments[index].split()[-1]
            if attribute.startswith("ExpressionPtr"):
                initialization = f"{attribute_name}(std::move({argument_name}))"
            else:
                initialization = f"{attribute_name}({argument_name})"
            initializations.append(initialization)

        expr_get_methods = [method_signature(item) for item in arguments]
        visitor_class = EXPRESSION_VISITOR_TEMPLATE.substitute(
            expr_class=expr_class,
            expr_get_methods="\t\t".join(expr_get_methods),
            arguments=", ".join(arguments),
            attributes = ";\n\t\t\t".join(attributes) + ";",
            initialization=", ".join(initializations),
        )

        classes += visitor_class

    forward_expression_signatures = [f"\tclass {expr};" for expr in grammar]
    expression_signatures = [f"\t\t\tvirtual void visit({expr}& expr) = 0;" for expr in grammar]
    expression_content = EXPRESSION_TEMPLATE.substitute(
        forward_expression_signatures="\n".join(forward_expression_signatures),
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
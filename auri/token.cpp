#include "auri/token.hpp"

namespace Auri {
Token::Token(TokenType type, std::string lexeme, TokenLiteral literal, int line) {
    type_ = type;
    lexeme_ = lexeme;
    literal_ = literal;
    line_ = line;
}
}    // namespace Auri

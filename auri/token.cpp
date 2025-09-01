#include "auri/token.hpp"

namespace Auri {
template <typename T>
Token<T>::Token(TokenType type, std::string lexeme, T literal, int line) {
  type_ = type;
  lexeme_ = lexeme;
  literal_ = literal;
  line_ = line;
}
}  // namespace Auri

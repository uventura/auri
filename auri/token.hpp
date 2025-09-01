#include <string>

namespace Auri {
enum class TokenType {
  // Math Operators
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,
  // Literals
  STRING,
  NUMBER,
  IDENTIFIER,
  // Variables
  GENERIC_VAR,
  NUMERIC_VAR,
  STRING_VAR,
  BOOL_VAR,
  // Keywords
  IF,
  ELSE,
  ELSE_IF,
  FOR,
  WHILE,
  // Execution Control
  SETUP,
  PRE_RUN,
  RUN,
  POST_RUN,
  // EOF
  AURI_EOF,
};

template <typename T>
class Token {
 private:
  TokenType type_;
  std::string lexeme_;
  T literal_;
  int line_;

 public:
  Token(TokenType type, std::string lexeme, T literal, int line);
};
}  // namespace Auri

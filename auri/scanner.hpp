#pragma once

namespace Auri {
enum class Tokens {
  // Math Operators
  SUM,
  SUB,
  DIV,
  MOD,
  EQUAL,
  // Variable types
  VAR,
  NUMERIC,
  STRING,
  BOOL,
  // Separators
  // Conditionals
  IF,
  ELSE,
  ELSE_IF,
  // Loops
  FOR,
  WHILE,
  // RunContext
  SETUP,
  PRE_RUN,
  RUN,
  POST_RUN
};
}

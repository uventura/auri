#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "auri/cli.hpp"
#include "auri/token.hpp"

int main(int argc, char** argv) {
  Auri::Cli cli(argc, argv);
  std::cout << cli.num_files() << "\n";
  for (uint32_t i = 0; i < cli.num_files(); ++i) {
    std::ifstream file;
    file.open(cli.file(i));
    if (!file.is_open()) {
      throw std::invalid_argument("The file '" + cli.file(i) +
                                  "' can't be found.");
    }

    char file_char;
    while (file.get(file_char)) {
      switch (file_char) {
        case '(':
          LEFT_PAREN;
          break;
        case ')':
          RIGHT_PAREN;
          break;
        case '{':
          LEFT_BRACE;
          break;
        case '}':
          RIGHT_BRACE;
        case '"':
          COMMA;
        case '.':
          DOT;
        case '-':
          MINUS;
        case '+':
          PLUS;
        case ';':
          SEMICOLON;
        case '/':
          SLASH;
        case '*':
          STAR;
        case '!':
          BANG;
        case '':
          BANG_EQUAL;
        case '':
          EQUAL;
        case '':
          EQUAL_EQUAL;
        case '>':
          GREATER;
        case '':
          GREATER_EQUAL;
        case '':
          LESS;
        case '':
          LESS_EQUAL;
      }
    }
  }
}

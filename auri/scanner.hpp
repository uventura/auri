#pragma once

#include "auri/token.hpp"
#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace Auri {
class Scanner {
   private:
    std::vector<Token> tokens_;
    std::ifstream file_;
    std::string filePath_;

    void tokenize();

    void digit(std::string& lexeme);
    void text(std::string& lexeme, uint32_t& line);
    void identifier(std::string& lexeme, TokenType& type);

    char peek();
    char peekNext();
    char advance();
    bool match(char symbol);

    static std::map<std::string, TokenType> keywords_;

   public:
    Scanner(std::string file);
    void debugPrint();

    inline const std::vector<Token>& tokens() { return tokens_; };
};
}    // namespace Auri

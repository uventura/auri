#pragma once

#include "auri/token.hpp"
#include <fstream>
#include <string>
#include <vector>

namespace Auri {
class Scanner {
   private:
    std::vector<Token> tokens_;
    std::ifstream file_;
    std::string filePath_;

    void tokenize();
    char peek();
    char advance();
    bool match(char symbol);

   public:
    Scanner(std::string file);

    inline const std::vector<Token>& tokens() {return tokens_;};
};
}    // namespace Auri

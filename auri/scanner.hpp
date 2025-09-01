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

    char peek();

   public:
    Scanner(std::string file);
};
}    // namespace Auri

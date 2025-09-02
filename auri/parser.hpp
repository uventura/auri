#pragma once

#include "auri/expression.hpp"
#include "auri/token.hpp"

#include <vector>

namespace Auri {
    class Parser {
        private:
            void parse();
        public:
            Parser(std::vector<Token>& tokens);
    };
}
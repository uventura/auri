#include "auri/cli.hpp"

#include <iostream>
#include <stdexcept>

namespace Auri {
std::string Cli::file(uint32_t index) { return files_[index]; }
uint32_t Cli::num_files() { return files_.size(); }

Cli::Cli(uint32_t argc, char** argv) {
    for (uint32_t i = 1; i < argc; ++i) {
        std::string element(argv[i]);

        if(element.size() > 2 && element[0] == '-' && element[1] == '-') {
            if(element == "--ast") {
                enableAstPrint_ = true;
                continue;
            }
            if (element == "--tokens") {
                enableTokenPrint_ = true;
                continue;
            }

            std::invalid_argument("Unknown option: " + element);
        }
        files_.push_back(element);
    }
}

bool Cli::enableAstPrint() {
    return enableAstPrint_;
}

bool Cli::enableTokenPrint() {
    return enableTokenPrint_;
}
}

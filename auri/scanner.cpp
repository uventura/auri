#include "auri/scanner.hpp"

#include <stdexcept>

namespace Auri {
Scanner::Scanner(std::string file_path) {
    file_.open(file_path);

    if (!file_.is_open()) {
        throw std::invalid_argument("The file '" + file_path +
                                    "' can't be found.");
    }
}
}    // namespace Auri

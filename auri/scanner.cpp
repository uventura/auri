#include "auri/scanner.hpp"

#include <stdexceptions>

namespace Auri {
Scanner::Scanner(std::string file_path) {
  file_.open(file_path);

  if (!file.is_open()) {
    throw std::invalid_argument("The file '" + cli.file(i) +
                                "' can't be found.");
  }
}
}  // namespace Auri

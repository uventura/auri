#pragma once

#include <array>
#include <cstdint>
#include <string>

#define MAX_FILES 100

namespace Auri {
enum class FileType { SETUP, PRE_RUN, RUN, POST_RUN };
struct File {
  std::string path;
  FileType type;
};
}  // namespace Auri

namespace Auri {
struct Cli {
 private:
  std::array<std::string, MAX_FILES> files_;

 public:
  Cli(uint32_t argc, char** argv);

  uint32_t num_files();
  std::string file(uint32_t index);
};

std::string Cli::file(uint32_t index) { return files_[index]; }
uint32_t Cli::num_files() { return files_.size(); }

Cli::Cli(uint32_t argc, char** argv) {
  for (uint32_t i = 1; i < argc; ++i) {
    std::string file(argv[i]);
    files_[i - 1] = file;
  }
}
}  // namespace Auri

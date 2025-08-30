#include <iostream>
#include <string>

#include "auri/cli.hpp"

int main(int argc, char** argv) {
  Auri::Cli cli(argc, argv);

  for (uint32_t i = 0; i < cli.num_files(); ++i) {
    std::cout << cli.file(i) << std::endl;
  }
}

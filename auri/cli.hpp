#pragma once

#include <cstdint>
#include <string>
#include <vector>

#define MAX_FILES 100

namespace Auri {
enum class FileType { SETUP, PRE_RUN, RUN, POST_RUN };
struct File {
    std::string path;
    FileType type;
};
}    // namespace Auri

namespace Auri {
struct Cli {
   private:
    std::vector<std::string> files_;
    bool enableAstPrint_{false};
    bool enableTokenPrint_{false};

   public:
    Cli(uint32_t argc, char** argv);

    uint32_t num_files();
    std::string file(uint32_t index);
    bool enableAstPrint();
    bool enableTokenPrint();
};
}    // namespace Auri

#pragma once

#include <map>
#include <string>

namespace Auri {
template <typename T>
class Environment {
   private:
    std::string identifier_;
    T value_;
    Environment* previousEnv_;

   public:
    Environment(std::string identifier, T value, Environment* previousEnv);

    T get(std::string identifier);
    void set(std::string identifier, T value);
};
}    // namespace Auri

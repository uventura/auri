#pragma once

#include <map>
#include <string>

namespace Auri {
class BaseEnvironment {};

template <typename T>
class Environment : public BaseEnvironment{
   private:
    std::map<std::string, T> values_;
    Environment* previousEnv_;

   public:
    // Environment(Environment* previousEnv) {}
    // Environment();
    template <typename T>
Environment<T>::Environment(Environment* previousEnv): previousEnv_(previousEnv) {}

template <typename T>
Environment<T>::Environment(): previousEnv_(nullptr) {}


    void declare(const std::string& id, T value) {
        values_[id] = value;
    }
    // void declare(std::string identifier, double value);
    // void declare(std::string identifier, std::string value);
    // void declare(std::string identifier, bool value);
    // void declare(std::string identifier, char value);

    T get(std::string identifier);
    void set(std::string identifier, T value);
};
}    // namespace Auri

#include "auri/environment.hpp"

#include <stdexcept>

namespace Auri {
template <typename T>
Environment<T>::Environment(Environment* previousEnv): previousEnv_(previousEnv) {}

template <typename T>
Environment<T>::Environment(): previousEnv_(nullptr) {}

// template <typename T>
// void Environment<T>::declare(std::string identifier, T value) {
//     values_[identifier] = value;
// }

template <typename T>
T Environment<T>::get(std::string identifier) {
    if(values_.find(identifier) == values_.end()) {
        if(previousEnv_ == nullptr) {
            throw std::runtime_error("The identifier '" + identifier + "' is not declared.");
        }

        return previousEnv_->get(identifier);
    }

    return values_[identifier];
}


// template <typename T>
// void Environment<T>::declare(std::string identifier, double value) {
//     values_[identifier] = value;
// }

// template <typename T>
// void Environment<T>::declare(std::string identifier, std::string value) {
//     values_[identifier] = value;
// }

// template <typename T>
// void Environment<T>::declare(std::string identifier, bool value) {
//     values_[identifier] = value;
// }

// template <typename T>
// void Environment<T>::declare(std::string identifier, char value) {
//     values_[identifier] = value;
// }

template <typename T>
void Environment<T>::set(std::string identifier, T value) {
    if(values_.find(identifier) == values_.end()) {
        if(previousEnv_ == nullptr) {
            throw std::runtime_error("The identifier '" + identifier + "' can't be set.");
        }

        previousEnv_->get(identifier);
        return;
    }

    values_[identifier] = value;
}
}    // namespace Auri

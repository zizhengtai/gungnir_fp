#ifndef GUNGNIR_DETAIL_LAZY_HPP
#define GUNGNIR_DETAIL_LAZY_HPP

#include <functional>
#include <utility>

#include "gungnir/detail/util.hpp"

namespace gungnir {

namespace detail {

template<typename T>
class Forward {
public:
    Forward(T &&val) noexcept : val_(std::forward<T>(val))
    {}

    operator T()
    {
        return std::forward<T>(val_);
    }

private:
    T val_;
};

}  // namespace detail

}  // namespace gungnir

#endif  // GUNGNIR_DETAIL_LAZY_HPP

/*
 * Copyright 2016 Zizheng Tai
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file gungnir/lazy.hpp
 * Utilities for lazy evaluation.
 */

#ifndef GUNGNIR_LAZY_HPP
#define GUNGNIR_LAZY_HPP

#include <functional>
#include <utility>

#include "gungnir/detail/lazy.hpp"

namespace gungnir {

using namespace detail;

/**
 * A lazily constructed value.
 *
 * @author Zizheng Tai
 * @since 1.0
 * @tparam T the type of the underlying value
 * @tparam Args the types of the arguments passed to the constructor of `T`
 */
template<typename T, typename... Args>
class LazyVal {
public:
    /**
     * Constructs a lazy value from the given arguments.
     *
     * @param args the arguments passed to the constructor of `T`
     */
    explicit LazyVal(Args&&... args) noexcept
        : args_(Forward<Args>(std::forward<Args>(args))...)
    {}

    /** Deleted copy constructor. */
    LazyVal(const LazyVal &) = delete;

    /** Default move constructor. */
    LazyVal(LazyVal &&) = default;

    /** Deleted copy assignment operator. */
    LazyVal & operator=(const LazyVal &) = delete;

    /** Default move assignment operator. */
    LazyVal & operator=(LazyVal &&) = default;

    /**
     * Destructs this lazy value.
     */
    ~LazyVal()
    {
        if (val_ != nullptr) {
            val_->~T();
        }
    }

    /**
     * Returns the underlying value, constructing it if necessary.
     *
     * @return the underlying value
     */
    T & get()
    {
        if (val_ == nullptr) {
            create(typename GenSeq<sizeof... (Args)>::type());
        }
        return *val_;
    }

    /**
     * Returns the underlying value, constructing it if necessary.
     *
     * @return the underlying value
     */
    operator T &()
    {
        return get();
    }

private:
    template<std::size_t... S>
    void create(Seq<S...>)
    {
        val_ = new (&storage_) T(std::get<S>(args_)...);
    }

    std::tuple<Forward<Args>...> args_;
    typename std::aligned_storage<sizeof (T), alignof (T)>::type storage_;
    T *val_ = nullptr;
};

/**
 * Returns a value that will be lazily constructed from the given arguments.
 *
 * @tparam T the type of the underlying value
 * @tparam Args the types of the arguments passed to the constructor of `T`
 * @param args the arguments passed to the constructor of `T`
*/
template<typename T, typename... Args>
LazyVal<T, Args...> lazyVal(Args&&... args)
{
    return LazyVal<T, Args...>(std::forward<Args>(args)...);
}

/**
 * Returns a callable object resulting from partially applying `f` to `args`.
 *
 * @tparam Fn the type of the underlying callable object
 * @tparam Args the types of the arguments passed to `f`
 * @param f the underlying function
 * @param args the arguments passed to `f`
 * @return a callable object resulting from partially applying `f` to `args`
 */
template<typename Fn, typename... Args>
auto lazyEval(Fn f, Args&&... args) -> decltype(
        std::bind(
            std::move(f),
            Forward<Args>(std::forward<Args>(args))...))
{
    return std::bind(
            std::move(f),
            Forward<Args>(std::forward<Args>(args))...);
}

}  // namespace gungnir

#endif  // GUNGNIR_LAZY_HPP

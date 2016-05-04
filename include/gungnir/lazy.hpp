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

#include <memory>
#include <functional>
#include <type_traits>
#include <utility>

#include "gungnir/detail/util.hpp"

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
class LazyVal final {
public:
    /**
     * Constructs a lazy value from the given arguments.
     *
     * @param args the arguments passed to the constructor of `T`
     */
    explicit LazyVal(Args&&... args) noexcept
        : args_(new std::tuple<Decay<Args>...>(std::forward<Args>(args)...))
    {}

    /**
     * Destructs the underlying value, if needed.
     */
    ~LazyVal()
    {
        if (val_) {
            val_->~T();
        }
    }

    /** Deleted copy constructor. */
    LazyVal(const LazyVal &) = delete;

    /** Default move constructor. */
    LazyVal(LazyVal &&) = default;

    /** Deleted copy assignment operator. */
    LazyVal & operator=(const LazyVal &) = delete;

    /** Default move assignment operator. */
    LazyVal & operator=(LazyVal &&) = default;

    /**
     * Returns the underlying value, constructing it if necessary.
     *
     * @return the underlying value
     */
    const T & get() const
    {
        if (!val_) {
            create(typename GenSeq<sizeof... (Args)>::type());
        }
        return *val_;
    }

    /**
     * Returns the underlying value, constructing it if necessary.
     *
     * @return the underlying value
     */
    T & get()
    {
        return const_cast<T &>(
                static_cast<const LazyVal<T, Args...> *>(this)->get());
    }

    /**
     * Returns the underlying value, constructing it if necessary.
     *
     * @return the underlying value
     */
    operator const T &() const
    {
        return get();
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

    /**
     * Returns the underlying value, constructing it if necessary.
     *
     * @return the underlying value
     */
    const T & operator()() const
    {
        return get();
    }

    /**
     * Returns the underlying value, constructing it if necessary.
     *
     * @return the underlying value
     */
    T & operator()()
    {
        return get();
    }

private:
    template<std::size_t... S>
    void create(Seq<S...>) const
    {
        val_ = new (&buf_) T(std::move(std::get<S>(*args_))...);
        args_.reset(nullptr);
    }

    mutable std::unique_ptr<std::tuple<Decay<Args>...>> args_;
    mutable typename std::aligned_storage<sizeof (T), alignof (T)>::type buf_;
    mutable T *val_ = nullptr;
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

}  // namespace gungnir

#endif  // GUNGNIR_LAZY_HPP

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

#ifndef GUNGNIR_OPTION_HPP
#define GUNGNIR_OPTION_HPP

#include <iterator>
#include <type_traits>

#include "gungnir/detail/util.hpp"

namespace gungnir {

template<typename T> class Option;
template<typename T> class UnownedOption;

namespace detail {

template<typename T, typename Impl>
class OptionBase {
public:
    bool isEmpty() const;
    T * ptr() const;
    void clear();

    operator bool() const { return !impl().isEmpty(); }

    operator T *() { return impl().ptr(); }
    operator const T *() const { return impl().ptr(); }

    T & operator*() { return *impl().ptr(); }
    const T & operator*() const { return *impl().ptr(); }

    T * operator->() { return impl().ptr(); }
    const T * operator->() const { return impl().ptr(); }

    bool operator==(T *that) const { return impl().ptr() == that; }
    bool operator!=(T *that) const { return impl().ptr() != that; }
    bool operator==(const OptionBase &that) const { return impl().ptr() == that.impl().ptr(); }
    bool operator!=(const OptionBase &that) const { return impl().ptr() != that.impl().ptr(); }

    T & get() { return *impl().ptr(); }
    const T & get() const { return *impl().ptr(); }

    T & getOrElse(T &that) { return impl().isEmpty() ? that : get(); }
    const T & getOrElse(const T &that) const { return impl().isEmpty() ? that : get(); }
    T getOrElse(T &&that) const { return impl().isEmpty() ? that : get(); }

    OptionBase & orElse(OptionBase &that) { return impl().isEmpty() ? that : *this; }
    const OptionBase & orElse(const OptionBase &that) const { return impl().isEmpty() ? that : *this; }
    Impl orElse(Impl &&that) const { return impl().isEmpty() ? that : impl(); }

    template<typename Fn>
    void foreach(Fn f) const
    {
        if (!impl().isEmpty()) {
            f(get());
        }
    }

    template<typename Fn, typename U = Decay<Ret<Fn, T>>>
    Option<U> map(Fn f) const
    {
        return impl().isEmpty() ? Option<U>() : Option<U>(f(get()));
    }

    template<typename Fn>
    UnownedOption<T> filter(Fn p)
    {
        T *ptr = impl().ptr();
        return UnownedOption<T>(ptr && p(*ptr) ? ptr : nullptr);
    }

    template<typename Fn>
    UnownedOption<const T> filter(Fn p) const
    {
        T *ptr = impl().ptr();
        return UnownedOption<const T>(ptr && p(*ptr) ? ptr : nullptr);
    }

    template<typename Fn>
    Option<T> filterMove(Fn p)
    {
        if (impl().isEmpty()) {
            return Option<T>();
        }
        T t = std::move(get());
        impl().clear();
        return p(t) ? Option<T>(std::move(t)) : Option<T>();
    }

    template<
        typename Fn,
        typename Opt = Decay<Ret<Fn, T>>,
        typename U = typename HKT<Opt>::L,
        typename = typename std::enable_if<
            std::is_base_of<OptionBase<U, Opt>, Opt>::value
        >::type
    >
    Opt flatMap(Fn f) const
    {
        return impl().isEmpty() ? Opt() : f(get());
    }

    template<typename T1 = T, typename U = typename HKT<T1>::L>
    UnownedOption<U> flatten() const
    {
        return UnownedOption<U>(impl().isEmpty() ? nullptr : get().impl().ptr());
    }

    template<typename T1 = T, typename U = typename HKT<T1>::L>
    Option<U> flattenMove()
    {
        if (impl().isEmpty()) {
            return Option<U>();
        }
        Option<U> opt = std::move(get());
        impl().clear();
        return opt;
    }

    template<
        typename U,
        typename BinaryOp,
        typename = typename std::enable_if<
            std::is_same<U, T>::value || std::is_base_of<U, T>::value
        >::type
    >
    U fold(U z, BinaryOp op) const
    {
        return foldLeft(std::move(z), std::move(op));
    }

    template<typename U, typename BinaryOp>
    U foldLeft(U z, BinaryOp op) const
    {
        return impl().isEmpty() ? z : op(std::move(z), get());
    }

    template<typename U, typename BinaryOp>
    U foldRight(U z, BinaryOp op) const
    {
        return impl().isEmpty() ? z : op(get(), std::move(z));
    }

    UnownedOption<T> begin() { return UnownedOption<T>(impl().ptr()); }
    UnownedOption<T> end() { return UnownedOption<T>(); }

    UnownedOption<const T> begin() const { return UnownedOption<const T>(impl().ptr()); }
    UnownedOption<const T> end() const { return UnownedOption<const T>(); }

    UnownedOption<const T> cbegin() const { return begin(); }
    UnownedOption<const T> cend() const { return end(); }

private:
    template<typename, typename> friend class OptionBase;

    Impl & impl() { return static_cast<Impl &>(*this); }
    const Impl & impl() const { return static_cast<const Impl &>(*this); }
};

}  // namespace detail

using detail::OptionBase;

/**
 * An optional value with managed storage.
 *
 * @author Zizheng Tai
 * @since 1.0
 * @tparam T the type of the contained value
 */
template<typename T>
class Option final : public OptionBase<T, Option<T>> {
public:
    Option() noexcept : empty_(true) {}

    template<typename... Args>
    explicit Option(Args&&... args) noexcept
        : empty_(!(new (&storage_) T(std::forward<Args>(args)...)))
    {}

    Option(const Option &) = delete;

    Option(Option &&that) noexcept
        : empty_(that.empty_ || !(new (&storage_) T(std::move(that.get()))))
    {
        that.clear();
    }

    ~Option()
    {
        clear();
    }

    Option & operator=(const Option &) = delete;

    Option & operator=(Option &&that)
    {
        empty_ = that.empty_ || !(new (&storage_) T(std::move(that.get())));
        that.clear();
        return *this;
    }

    bool isEmpty() const
    {
        return empty_;
    }

    T * ptr() const
    {
        return empty_ ? nullptr : reinterpret_cast<T *>(&storage_);
    }

    void clear()
    {
        if (!empty_) {
            OptionBase<T, Option>::get().~T();
            empty_ = true;
        }
    }

private:
    bool empty_;
    mutable typename std::aligned_storage<sizeof (T), alignof (T)>::type storage_;
};

/**
 * An optional value with unmanaged storage.
 *
 * @author Zizheng Tai
 * @since 1.0
 * @tparam T the type of the contained value
 */
template<typename T>
class UnownedOption final
    : public OptionBase<T, UnownedOption<T>>
    , public std::iterator<std::forward_iterator_tag, T> {

public:
    explicit UnownedOption(T *ptr = nullptr) noexcept : ptr_(ptr) {}

    UnownedOption(const UnownedOption &) = default;

    UnownedOption(UnownedOption &&that) noexcept : ptr_(that.ptr_)
    {
        that.ptr_ = nullptr;
    }

    UnownedOption & operator=(const UnownedOption &) = default;

    UnownedOption & operator=(UnownedOption &&that)
    {
        ptr_ = that.ptr_;
        that.ptr_ = nullptr;
    }

    UnownedOption & operator++()
    {
        ptr_ = nullptr;
        return *this;
    }

    UnownedOption operator++(int)
    {
        T *old = ptr_;
        ptr_ = nullptr;
        return UnownedOption(old);
    }

    bool isEmpty() const
    {
        return !ptr_;
    }

    T * ptr() const
    {
        return ptr_;
    }

    void clear()
    {
        ptr_ = nullptr;
    }

private:
    T *ptr_;
};

}  // namespace gungnir

#endif  // GUNGNIR_OPTION_HPP

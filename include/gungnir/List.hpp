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

#ifndef GUNGNIR_LIST_HPP
#define GUNGNIR_LIST_HPP

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include "gungnir/detail/util.hpp"

namespace gungnir {

using namespace detail;

/**
 * An immutable linked list.
 *
 * @author Zizheng Tai
 * @since 1.0
 * @tparam A the type of the elements; must be a non-reference type
 */
template<typename A>
class List final {

    class Node;

    template<typename T>
    using Ptr = std::shared_ptr<const T>;

public:
    /**
     * @brief Constructs an empty list.
     */
    List() noexcept : node_(Node::create())
    {}

    /**
     * @brief Constructs a list with the given element.
     *
     * @param x the only element of this list
     */
    List(A x) noexcept
        : node_(Node::create(
                    std::make_shared<const A>(std::move(x)),
                    Node::create()))
    {}

    /**
     * @brief Constructs a list with the given head and tail.
     *
     * @param head the first element of this list
     * @param tail all elements of this list except the first one
     */
    List(A head, List tail) noexcept
        : node_(Node::create(
                    std::make_shared<const A>(std::move(head)),
                    std::move(tail.node_)))
    {}

    /**
     * @brief Constructs a list with the given elements.
     *
     * @tparam Args the types of the given elements
     * @param head the first element of this list
     * @param tail all elements of this list except the first one
     */
    template<
        typename... Args,
        typename = typename std::enable_if<
            AllTrue<std::is_convertible<Args, A>::value...>::value
        >::type
    >
    List(A head, Args&&... tail) noexcept
        : List(std::move(head), List(std::forward<Args>(tail)...))
    {}

    /** @brief Default copy constructor. */
    List(const List &) = default;

    /** @brief Default move constructor. */
    List(List &&) = default;

    /** @brief Default copy assignment operator. */
    List & operator=(const List &) = default;

    /** @brief Default move assignment operator. */
    List & operator=(List &&) = default;

    /**
     * @brief Returns `true` if this list contains no elements, `false` otherwise.
     *
     * @return `true` if this list contains no elements, `false` otherwise
     */
    bool isEmpty() const
    {
        return node_->size == 0;
    }

    /**
     * @brief Returns the number of elements of this list.
     *
     * @return the number of elements of this list
     */
    std::size_t size() const
    {
        return node_->size;
    }

    /**
     * @brief Returns the first element of this list.
     *
     * @return the first element of this list
     * @throws std::out_of_range if this list is empty
     */
    const A & head() const
    {
        if (isEmpty()) {
            throw std::out_of_range("head of empty list");
        }
        return *(node_->head);
    }

    /**
     * @brief Returns all elements of this list except the first one.
     *
     * @return all elements of this list except the first one
     * @throws std::out_of_range if this list is empty
     */
    List tail() const
    {
        if (isEmpty()) {
            throw std::out_of_range("tail of empty list");
        }
        return node_->tail;
    }

    /**
     * @brief Returns the last element of this list.
     *
     * @return the last element of this list
     * @throws std::out_of_range if this list is empty
     */
    const A & last() const
    {
        if (isEmpty()) {
            throw std::out_of_range("last of empty list");
        }
        return (*this)[size() - 1];
    }

    /**
     * @brief Returns all elements of this list except the last one.
     *
     * @return all elements of this list except the last one
     * @throws std::out_of_range if this list is empty
     */
    List init() const
    {
        if (isEmpty()) {
            throw std::out_of_range("init of empty list");
        }
        return take(size() - 1);
    }

    /**
     * Applies a function to each element of this list.
     *
     * @param f the function to apply, for its side-effect,
     *          to each element of this list
     */
    template<typename Fn>
    void foreach(Fn f) const
    {
        foreachImpl([&f](const Ptr<A> &x) { f(*x); });
    }

    /**
     * @brief Returns a new list resulting from applying a function to
     *        each element of this list.
     *
     * @tparam Fn the type of the function to apply to each element of this list
     * @tparam B the element type of returned list
     * @param f the function to apply to each element of this list
     * @return a new list resulting from applying the given function `f` to
     *         each element of this list
     */
    template<typename Fn, typename B = Decay<Ret<Fn, A>>>
    List<B> map(Fn f) const
    {
        const auto ff = std::bind(std::move(f), std::placeholders::_1);

        std::vector<Ptr<B>> buf;
        buf.reserve(size());
        foreachImpl([&buf, &ff](const Ptr<A> &x) {
            buf.emplace_back(std::make_shared<const B>(ff(*x)));
        });

        return List<B>::toList(
                std::make_move_iterator(buf.rbegin()),
                std::make_move_iterator(buf.rend()));
    }

    /**
     * @brief Returns all elements of this list that satisfy a predicate.
     *
     * @tparam Fn type of the predicate
     * @param p the predicate used to test elements
     * @return a new list consisting of all elements of this list that satisfy
     *         the given predicate `p`. The order of the elements is preserved.
     */
    template<typename Fn>
    List filter(Fn p) const
    {
        std::vector<Ptr<A>> buf;
        foreachImpl([&p, &buf](const Ptr<A> &x) {
            if (p(*x)) {
                buf.emplace_back(x);
            }
        });

        return toList(
                std::make_move_iterator(buf.rbegin()),
                std::make_move_iterator(buf.rend()));
    }

    /**
     * @brief Returns a new list with elements of this list in reversed order.
     *
     * @return a new list with elements of this list in reversed order
     */
    List reverse() const
    {
        auto hd = Node::create();
        foreachImpl([&hd](const Ptr<A> &x) {
            hd = Node::create(x, std::move(hd));
        });
        return hd;
    }

    /**
     * @brief Returns the first `n` elements of this list.
     *
     * @param n the number of elements to take
     * @return a list consisting of the first `n` elements of this list,
     *         or the whole list if `n > size()`
     */
    List take(std::size_t n) const
    {
        n = std::min(n, size());

        std::vector<Ptr<A>> buf;
        for (auto nd = node_.get(); n > 0; nd = nd->tail.get(), --n) {
            buf.emplace_back(nd->head);
        }

        return toList(
                std::make_move_iterator(buf.rbegin()),
                std::make_move_iterator(buf.rend()));
    }

    /**
     * @brief Returns the last `n` elements of this list.
     *
     * @param n the number of elements to take
     * @return a list consisting of the last `n` elements of this list,
     *         or the whole list if `n > size()`
     */
    List takeRight(std::size_t n) const
    {
        return drop(size() - std::min(n, size()));
    }

    /**
     * @brief Returns the longest prefix of this list whose elements satisfy
     *        the given predicate.
     *
     * @tparam Fn the type of the predicate
     * @param p the predicate
     * @return the longest prefix of this list whose elements satisfy
     *         the given predicate
     */
    template<typename Fn>
    List takeWhile(Fn p) const
    {
        std::size_t num = 0;
        for (auto n = node_.get();
                n->size > 0 && p(*(n->head));
                n = n->tail.get(), ++num) {
        }
        return take(num);
    }

    /**
     * @brief Returns all elements of this list except the first `n` ones.
     *
     * @param n the number of elements to drop
     * @return a list consisting of all elements of this list except
     *         the first `n` ones, or an empty list if `n > size()`
     */
    List drop(std::size_t n) const
    {
        if (n >= size()) {
            return List();
        }
        auto pn = &node_;
        for (; n > 0; pn = &((*pn)->tail), --n) {}
        return *pn;
    }

    /**
     * @brief Returns all elements of this list except the last `n` ones.
     *
     * @param n the number of elements to drop
     * @return a list consisting of all elements of this list except
     *         the last `n` ones, or an empty list if `n > size()`
     */
    List dropRight(std::size_t n) const
    {
        return take(size() - std::min(n, size()));
    }

    /**
     * @brief Returns the longest suffix of this list whose first element
     *        does not satisfy the given predicate.
     *
     * @tparam Fn the type of the predicate
     * @param p the predicate
     * @return the longest suffix of this list whose first element does not
     *         satisfy the given predicate
     */
    template<typename Fn>
    List dropWhile(Fn p) const
    {
        std::size_t num = 0;
        for (auto n = node_.get();
                n->size > 0 && p(*(n->head));
                n = n->tail.get(), ++num) {
        }
        return drop(num);
    }

    /**
     * @brief Returns a list consisting of all elements of this list starting at
     *        position `from` and extending up until position `until`.
     *
     * An empty list is returned if `from >= until` or `from >= size()`.
     *
     * @param from the index of the starting position (included)
     * @param until the index of the ending position (excluded)
     * @return a list consisting of all elements of this list starting at
     *         position `from` and extending up until position `until`,
     *         or an empty list if `from >= until` or `from >= size()`
     */
    List slice(std::size_t from, std::size_t until) const
    {
        if (from >= until) {
            return List();
        }
        return drop(from).take(until - from);
    }

    /**
     * @brief Returns a list resulting from applying the given function `f`
     *        to each element of this list and concatenating the results.
     *
     * @tparam Fn the type of the function to apply to each element of this list
     * @tparam B the element type of the returned list
     * @return a list resulting from applying the given function `f` to each
     *         element of this list and concatenating the results
     */
    template<typename Fn, typename B = Decay<typename HKT<Ret<Fn, A>>::L>>
    List<B> flatMap(Fn f) const
    {
        std::vector<Ptr<B>> buf;
        foreachImpl([&f, &buf](const Ptr<A> &x) {
            f(*x).foreachImpl([&buf](const Ptr<B> &y) {
                buf.emplace_back(y);
            });
        });

        return List<B>::toList(
                std::make_move_iterator(buf.rbegin()),
                std::make_move_iterator(buf.rend()));
    }

    /**
     * @brief Returns a list resulting from concatenating all element lists
     *        of this list.
     *
     * @tparam A1 the same as A, used to make SFINAE work
     * @tparam B the element type of the returned list
     * @return a list resulting from concatenating all element lists of this list.
     */
    template<typename A1 = A, typename B = typename HKT<A1>::L>
    List<B> flatten() const
    {
        return flatMap(identity<const List<B> &>);
    }

    /**
     * @brief Returns `true` if at least one element of this list satisfy
     *        the given predicate, `false` otherwise.
     *
     * @tparam Fn the type of the predicate
     * @param p the predicate
     * @return `true` if at least one element of this list satisfy the given
     *         predicate, `false` otherwise
     */
    template<typename Fn>
    bool exists(Fn p) const
    {
        for (auto n = node_.get(); n->size > 0; n = n->tail.get()) {
            if (p(*(n->head))) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Returns `true` if this list is empty or the given predicate
     *        holds for all elements of this list, `false` otherwise.
     *
     * @tparam Fn the type of the predicate
     * @param p the predicate
     * @return `true` if this list is empty or the given predicate holds for
     *         all elements of this list, `false` otherwise
     */
    template<typename Fn>
    bool forall(Fn p) const
    {
        for (auto n = node_.get(); n->size > 0; n = n->tail.get()) {
            if (!p(*(n->head))) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Returns `true` if this list has an element that is equal
     *        (as determined by `==`) to `x`, `false` otherwise.
     *
     * @param x the object to test against
     * @return `true` if this list has an element that is equal
     *         (as determined by `==`) to `x`, `false` otherwise
     */
    bool contains(const A &x) const
    {
        for (auto n = node_.get(); n->size > 0; n = n->tail.get()) {
            if (*(n->head) == x) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Returns the number of elements of this list that are equal
     *        (as determined by `==`) to `x`.
     *
     * @param x the object to test against
     * @return the number of elements of this list that are equal
     *         (as determined by `==`) to `x`
     */
    std::size_t count(const A &x) const
    {
        std::size_t num = 0;
        foreachImpl([&x, &num](const Ptr<A> &y) {
            if (*y == x) {
                ++num;
            }
        });
        return num;
    }

    /**
     * @brief Returns the number of elements of this list that satisfy
     *        the given predicate.
     *
     * @tparam Fn the type of the predicate
     * @param p the predicate
     * @return the number of elements of this list that satisfy
     *         the given predicate
     */
    template<typename Fn>
    std::size_t count(Fn p) const
    {
        std::size_t num = 0;
        foreachImpl([&p, &num](const Ptr<A> &y) {
            if (p(*y)) {
                ++num;
            }
        });
        return num;
    }

    /**
     * @brief Returns a list whose head is constructed in-place from `args`,
     *        and tail is this list.
     *
     * @tparam Args the types of the arguments passed to the constructor of `A`
     * @param args the arguments passed to the constructor of `A`
     * @return a list whose head is constructed in-place from `args`,
     *         and tail is this list
     */
    template<typename... Args>
    List prepend(Args&&... args) const
    {
        return Node::create(
                std::make_shared<A>(std::forward<Args>(args)...),
                node_);
    }

    /**
     * @brief Returns a list resulting from concatenating this list and `that`.
     *
     * @param that the list whose elements follow those of this list
     *             in the returned list
     * @return a list resulting from concatenating this list and `that`
     */
    List concat(const List &that) const
    {
        if (isEmpty()) {
            return that;
        } else if (that.isEmpty()) {
            return *this;
        }

        std::vector<Ptr<A>> buf;
        buf.reserve(size());
        foreachImpl([&buf](const Ptr<A> &x) {
            buf.emplace_back(x);
        });

        return toList(
                std::make_move_iterator(buf.rbegin()),
                std::make_move_iterator(buf.rend()),
                that.node_);
    }

    /**
     * @brief Returns a copy of this list with one single replaced element.
     *
     * @tparam Args the types of the argument passed to the constructor of `A`
     * @param index the position of the replacement
     * @param args the argument passed to the constructor of `A`
     * @return a copy of this list with the element at position `index`
     *         replaced by a new element constructed in-place from `args`
     * @throws std::out_of_range if `index >= size()`
     */
    template<typename... Args>
    List updated(std::size_t index, Args&&... args) const
    {
        if (index >= size()) {
            throw std::out_of_range("index out of range");
        }

        std::vector<Ptr<A>> buf;
        buf.reserve(index);
        auto n = node_.get();
        for (; index > 0; n = n->tail.get(), --index) {
            buf.emplace_back(n->head);
        }

        return toList(
                std::make_move_iterator(buf.rbegin()),
                std::make_move_iterator(buf.rend()),
                Node::create(
                    std::make_shared<A>(std::forward<Args>(args)...),
                    n->tail));
    }

    /**
     * @brief Folds the elements of this list using the specified associative
     *        binary operator.
     *
     * The order in which operations are performed on elements is unspecified
     * and may be nondeterministic.
     *
     * @tparam A1 the result type of the binary operator, a supertype of `A`
     * @tparam Fn the type of the associative binary operator
     * @param z a neutral element for the fold operation; may be added to
     *          the result an arbitrary number of times, and must not change
     *          the result (e.g., an empty list for list concatenation,
     *          0 for addition, or 1 for multiplication)
     * @param op a binary operator that must be associative
     * @return the result of applying the fold operator `op` between all
     *         elements of this list and `z`, or `z` if this list is empty
     */
    template<
        typename A1,
        typename Fn,
        typename = typename std::enable_if<
            std::is_same<A1, A>::value ||
            std::is_base_of<A1, A>::value
        >::type
    >
    A1 fold(A1 z, Fn op) const
    {
        return foldLeft(std::move(z), std::move(op));
    }

    /**
     * @brief Applies a binary operator to a start value and all elements of
     *        this list, going left to right.
     *
     * @tparam B the result type of the binary operator
     * @tparam Fn the type of the binary operator
     * @param z the start value
     * @param op the binary operator
     * @return the result of inserting `op` between consecutive elements of
     *         this list, going left to right with the start value `z`
     *         on the left, or `z` if this list is empty
     */
    template<typename B, typename Fn>
    B foldLeft(B z, Fn op) const
    {
        foreachImpl([&z, &op](const Ptr<A> &x) {
            z = op(std::move(z), *x);
        });
        return z;
    }

    /**
     * @brief Applies a binary operator to a start value and all elements of
     *        this list, going right to left.
     *
     * @tparam B the result type of the binary operator
     * @tparam Fn the type of the binary operator
     * @param z the start value
     * @param op the binary operator
     * @return the result of inserting `op` between consecutive elements of
     *         this list, going right to left with the start value `z`
     *         on the right, or `z` if this list is empty
     */
    template<typename B, typename Fn>
    B foldRight(B z, Fn op) const
    {
        std::vector<const A *> buf;
        buf.reserve(size());
        foreachImpl([&buf](const Ptr<A> &x) {
            buf.emplace_back(&*x);
        });

        for (auto it = buf.crbegin(); it != buf.crend(); ++it) {
            z = op(**it, std::move(z));
        }
        return z;
    }

    /**
     * @brief Returns the sum of all elements of this list,
     *        or 0 if this list is empty.
     *
     * @return the sum of all elements of this list, or 0 if this list is empty
     */
    A sum() const
    {
        A acc = 0;
        foreachImpl([&acc](const Ptr<A> &x) {
            acc += *x;
        });
        return acc;
    }

    /**
     * @brief Returns the product of all elements of this list,
     *        or 1 if this list is empty.
     *
     * @return the product of all elements of this list,
     *         or 1 if this list is empty
     */
    A product() const
    {
        A acc = 1;
        foreachImpl([&acc](const Ptr<A> &x) {
            acc *= *x;
        });
        return acc;
    }

    /**
     * @brief Returns a list consisting of elements of this list sorted in
     *        ascending order, as determined by the `<` operator.
     *
     * The sort is stable. That is, equal elements appear in the same order
     * in the sorted list as in the original.
     *
     * @return a list consisting of elements of this list sorted in
     *         ascending order, as determined by the `<` operator
     */
    List sorted() const
    {
        return sorted([](const A &x, const A &y) { return x < y; });
    }

    /**
     * @brief Returns a list consisting of elements of this list sorted in
     *        ascending order, as determined by the given comparator.
     *
     * The sort is stable. That is, equal elements appear in the same order
     * in the sorted list as in the original.
     *
     * @tparam Fn the type of the comparator
     * @param lt a comparator that returns `true` if its first argument
     *           is *less* than (i.e., is ordered *before*) the second
     * @return a list consisting of elements of this list sorted in
     *         ascending order, as determined by the given comparator
     */
    template<typename Fn>
    List sorted(Fn lt) const
    {
        std::vector<Ptr<A>> buf;
        buf.reserve(size());
        foreachImpl([&buf](const Ptr<A> &x) {
            buf.emplace_back(x);
        });

        std::stable_sort(
                buf.begin(),
                buf.end(),
                [&lt](const Ptr<A> &x, const Ptr<A> &y) { return lt(*x, *y); });

        return toList(
                std::make_move_iterator(buf.rbegin()),
                std::make_move_iterator(buf.rend()));
    }

    /**
     * @brief Returns a list resulting from wrapping the elements of this list
     *        in `std::reference_wrapper<const A>`s.
     *
     * @return a list resulting from wrapping the elements of this list
     *         in `std::reference_wrapper<const A>`s
     */
    List<std::reference_wrapper<const A>> cref() const
    {
        return map([](const A &x) { return std::cref(x); });
    }

    /**
     * @brief Returns a list formed from this list and `that` by combining
     *        corresponding elements in pairs.
     *
     * If one of the two lists is longer than the other, its remaining elements
     * are ignored.
     *
     * The list elements are copied into the pairs. To avoid copying, transform
     * the lists with `cref()` and zip the resulting lists instead.
     *
     * @tparam B the element type of `that`
     * @param that the list providing the second element of each result pair
     * @return a list formed from this list and `that` by combining
     *         corresponding elements in pairs
     */
    template<typename B>
    List<std::pair<A, B>> zip(const List<B> &that) const
    {
        using AB = std::pair<A, B>;

        std::size_t s = std::min(size(), that.size());
        std::vector<Ptr<AB>> buf;
        buf.reserve(s);

        auto n1 = node_.get();
        auto n2 = that.node_.get();
        for (; s > 0; n1 = n1->tail.get(), n2 = n2->tail.get(), --s) {
            buf.emplace_back(std::make_shared<AB>(*(n1->head), *(n2->head)));
        }

        return List<AB>::toList(
                std::make_move_iterator(buf.rbegin()),
                std::make_move_iterator(buf.rend()));
    }

    /**
     * @brief Returns a list consisting of the intermediate results of
     *        a left fold over this list with the given start value and
     *        binary operator.
     *
     * @tparam B the result type of the binary operator
     * @tparam Fn the type of the binary operator
     * @param z the start value
     * @param op the binary operator
     * @return a list consisting of the intermediate results of a left fold
     *         over this list with the given start value and binary operator
     */
    template<typename B, typename Fn>
    List<B> scanLeft(B z, Fn op) const
    {
        std::vector<Ptr<B>> buf;
        buf.reserve(size() + 1);
        buf.emplace_back(std::make_shared<B>(std::move(z)));
        foreachImpl([&buf, &op] (const Ptr<A> &x) {
            buf.emplace_back(std::make_shared<B>(op(*buf.back(), *x)));
        });

        return List<B>::toList(
                std::make_move_iterator(buf.rbegin()),
                std::make_move_iterator(buf.rend()));
    }

    /**
     * @brief Returns the element at the specified position of this list.
     *
     * @param index index of the element to return
     * @return the element at the specified position of this list
     * @throws std::out_of_range if `index` is out of range (`index >= size()`)
     */
    const A & operator[](std::size_t index) const
    {
        if (index >= size()) {
            throw std::out_of_range("index out of range");
        }
        auto n = node_.get();
        for (; index > 0; n = n->tail.get(), --index) {}
        return *(n->head);
    }

    /**
     * @brief Compares this list with the given list for equality.
     *
     * @param that the list to be compared for equality with this list
     * @return `true` if `that` contains the same elements as this list
     *         in the same order, `false` otherwise
     */
    bool operator==(const List<A> &that) const
    {
        if (this == &that) {
            return true;
        }
        if (size() != that.size()) {
            return false;
        }
        for (auto n1 = node_.get(), n2 = that.node_.get();
                n1->size != 0;
                n1 = n1->tail.get(), n2 = n2->tail.get()) {
            if (*(n1->head) != *(n2->head)) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Compares this list with the given list for inequality.
     *
     * @param that the list to be compared for inequality with this list
     * @return `true` if `that` does not contains the same elements as this list
     *         in the same order, `false` otherwise
     */
    bool operator!=(const List<A> &that) const
    {
        return !(*this == that);
    }

private:
    template<typename>
    friend class List;

    class Node final {
        class Priv final {};

    public:
        static Ptr<Node> create()
        {
            return std::make_shared<const Node>(Priv());
        }

        static Ptr<Node> create(Ptr<A> head, Ptr<Node> tail)
        {
            return std::make_shared<const Node>(
                    Priv(), std::move(head), std::move(tail));
        }

        Node(Priv) noexcept : size(0)
        {}

        Node(Priv, Ptr<A> head, Ptr<Node> tail) noexcept
            : size(1 + tail->size)
            , head(std::move(head))
            , tail(std::move(tail))
        {}

        const std::size_t size;
        const Ptr<A> head;
        const Ptr<Node> tail;
    };

    List(Ptr<Node> node) noexcept : node_(std::move(node))
    {}

    template<typename Fn>
    void foreachImpl(Fn f) const
    {
        for (auto n = node_.get(); n->size > 0; n = n->tail.get()) {
            f(n->head);
        }
    }

    template<typename RevIter>
    static List toList(
            RevIter rbegin,
            RevIter rend,
            Ptr<Node> head = Node::create())
    {
        for (; rbegin != rend; ++rbegin) {
            head = Node::create(*rbegin, std::move(head));
        }
        return head;
    }

    Ptr<Node> node_;
};

}  // namespace gungnir

#endif  // GUNGNIR_LIST_HPP

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
#include <vector>

#include "gungnir/util.hpp"

namespace gungnir {

using namespace detail;

/**
 * An immutable linked list.
 *
 * @author Zizheng Tai
 * @since 1.0
 * @tparam A type of elements
 */
template<typename A>
class List final {
private:
    class Node;

    template<typename T>
    using Ptr = std::shared_ptr<const T>;

public:
    /**
     * Constructs an empty list.
     */
    List() noexcept : node_(Node::create())
    {}

    /**
     * Constructs a list with the given element.
     *
     * @param x the only element of this list
     */
    List(A x) noexcept
        : node_(Node::create(
                    std::make_shared<const A>(std::move(x)),
                    Node::create()))
    {}

    /**
     * Constructs a list with the given head and tail.
     *
     * @param head the first element of this list
     * @param tail all except the first elements of this list
     */
    List(A head, List tail) noexcept
        : node_(Node::create(
                    std::make_shared<const A>(std::move(head)),
                    std::move(tail.node_)))
    {}

    /**
     * Constructs a list with the given elements.
     *
     * @tparam Args types of the given elements
     * @param head the first element of this list
     * @param tail all except the first elements of this list
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

    /** Default copy constructor. */
    List(const List &) = default;

    /** Default move constructor. */
    List(List &&) = default;

    /** Default copy assignment operator. */
    List & operator=(const List &) = default;

    /** Default move assignment operator. */
    List & operator=(List &&) = default;

    /**
     * Returns `true` if this list contains no elements, `false` otherwise.
     *
     * @return `true` if this list contains no elements, `false` otherwise
     */
    bool isEmpty() const
    {
        return node_->size == 0;
    }

    /**
     * Returns the number of elements of this list.
     *
     * @return the number of elements of this list
     */
    std::size_t size() const
    {
        return node_->size;
    }

    /**
     * Returns the first element of this list.
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
     * Returns all except the first elements of this list.

     * @return all except the first elements of this list
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
     * Applies a function to every element of this list.
     *
     * @param f the function to apply to every element for its side-effect
     */
    template<typename Fn>
    void foreach(Fn f) const
    {
        foreachImpl([&f](const Ptr<A> &x) { f(*x); });
    }

    /**
     * Returns a new list resulting from applying a function to
     * every element of this list.
     *
     * @tparam Fn type of the function to apply to every element
     * @tparam B type of elements of the resulting list
     * @param f the function to apply to every element
     * @return a new list resulting from applying the given function `f` to
     *         every element of this list
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

        using BN = typename List<B>::Node;
        auto hd = BN::create();
        for (auto it = buf.rbegin(); it != buf.rend(); ++it) {
            hd = BN::create(std::move(*it), std::move(hd));
        }

        return hd;
    }

    /**
     * Returns all elements of this list that satisfy a predicate.
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

        auto hd = Node::create();
        for (auto it = buf.rbegin(); it != buf.rend(); ++it) {
            hd = Node::create(std::move(*it), std::move(hd));
        }

        return hd;
    }

    /**
     * Returns a new list with elements of this list in reversed order.
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
     * Returns the first `n` elements of this list.
     *
     * @param n the number of elements to take from this list
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

        auto hd = Node::create();
        for (auto it = buf.rbegin(); it != buf.rend(); ++it) {
            hd = Node::create(std::move(*it), std::move(hd));
        }

        return hd;
    }

    /**
     * Returns all except the first `n` elements of this list.
     *
     * @param n the number of elements to drop from this list
     * @return a list consisting of all except the first `n` elements of
     *         this list, or an empty list if `n > size()`.
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
     * Returns the element at the specified position of this list.
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
     * Compares this list with the given list for equality.
     *
     * @param that the list to be compared for equality with this list
     * @return `true` if `that` contains the same elements as this list
     *         in the same order, `false` otherwise.
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
     * Compares this list with the given list for inequality.
     *
     * @param that the list to be compared for inequality with this list
     * @return `true` if `that` does not contains the same elements as this list
     *         in the same order, `false` otherwise.
     */
    bool operator!=(const List<A> &that) const
    {
        return !(*this == that);
    }

private:
    template<typename>
    friend class List;

    class Node final {
    private:
        static const class Priv final {} priv_;

    public:
        static Ptr<Node> create()
        {
            return std::make_shared<const Node>(priv_);
        }

        static Ptr<Node> create(Ptr<A> head, Ptr<Node> tail)
        {
            return std::make_shared<const Node>(
                priv_, std::move(head), std::move(tail));
        }

        Node(const Priv &) noexcept : size(0)
        {}

        Node(const Priv &, Ptr<A> head, Ptr<Node> tail) noexcept
            : size(1 + tail->size)
            , head(std::move(head))
            , tail(std::move(tail))
        {}

        const std::size_t size;
        const Ptr<A> head;
        const Ptr<Node> tail;
    };

    List(Ptr< Node> node) noexcept : node_(std::move(node))
    {}

    template<typename Fn>
    void foreachImpl(Fn f) const
    {
        for (auto n = node_.get(); n->size > 0; n = n->tail.get()) {
            f(n->head);
        }
    }

    Ptr<Node> node_;
};

template<typename A>
const typename List<A>::Node::Priv List<A>::Node::priv_{};

}  // namespace gungnir

#endif  // GUNGNIR_LIST_HPP

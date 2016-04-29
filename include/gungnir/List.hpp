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

template<typename A>
class List final {
private:
    class Node;

    template<typename T>
    using Ptr = std::shared_ptr<const T>;

public:
    List() noexcept : node_(Node::create())
    {}

    List(A x) noexcept
        : node_(Node::create(
                    std::make_shared<const A>(std::move(x)),
                    Node::create()))
    {}

    List(A head, List tail) noexcept
        : node_(Node::create(
                    std::make_shared<const A>(std::move(head)),
                    std::move(tail.node_)))
    {}

    template<
        typename... Args,
        typename = typename std::enable_if<
            AllTrue<std::is_convertible<Args, A>::value...>::value
        >::type
    >
    List(A head, Args&&... tail) noexcept
        : List(std::move(head), List(std::forward<Args>(tail)...))
    {}

    List(Ptr< Node> node) noexcept : node_(std::move(node))
    {}

    bool isEmpty() const
    {
        return node_->size == 0;
    }

    std::size_t size() const
    {
        return node_->size;
    }

    const A & head() const
    {
        if (isEmpty()) {
            throw std::out_of_range("head of empty list");
        }
        return *(node_->head);
    }

    List tail() const
    {
        if (isEmpty()) {
            throw std::out_of_range("tail of empty list");
        }
        return node_->tail;
    }

    template<typename Fn>
    void foreach(Fn f) const
    {
        for (auto n = node_.get(); n->size > 0; n = n->tail.get()) {
            f(*(n->head));
        }
    }

    template<typename Fn, typename B = Decay<Ret<Fn, A>>>
    List<B> map(Fn f) const
    {
        const auto ff = std::bind(std::move(f), std::placeholders::_1);

        std::vector<Ptr<B>> buf;
        buf.reserve(size());

        foreach([&buf, &ff](const A &x) {
            buf.emplace_back(std::make_shared<const B>(ff(x)));
        });

        using BN = typename List<B>::Node;
        auto hd = BN::create();
        for (auto it = buf.rbegin(); it != buf.rend(); ++it) {
            hd = BN::create(std::move(*it), std::move(hd));
        }

        return hd;
    }

    template<typename Fn>
    List filter(Fn p) const
    {
        std::vector<Ptr<A>> buf;

        for (auto n = node_.get(); n->size > 0; n = n->tail.get()) {
            if (p(*(n->head))) {
                buf.emplace_back(n->head);
            }
        }

        auto hd = Node::create();
        for (auto it = buf.rbegin(); it != buf.rend(); ++it) {
            hd = Node::create(std::move(*it), std::move(hd));
        }

        return hd;
    }

    List reverse() const
    {
        auto hd = Node::create();
        for (auto n = node_.get(); n->size > 0; n = n->tail.get()) {
            hd = Node::create(n->head, std::move(hd));
        }
        return hd;
    }

    const A & operator[](std::size_t index) const
    {
        if (index >= size()) {
            throw std::out_of_range("index out of range");
        }
        auto n = node_.get();
        for (; index > 0; n = n->tail.get(), --index) {}
        return *(n->head);
    }

    bool operator==(const List<A> &rhs) const
    {
        if (this == &rhs) {
            return true;
        }
        if (size() != rhs.size()) {
            return false;
        }
        for (auto n1 = node_.get(), n2 = rhs.node_.get();
                n1->size != 0;
                n1 = n1->tail.get(), n2 = n2->tail.get()) {
            if (*(n1->head) != *(n2->head)) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const List<A> &rhs) const
    {
        return !(*this == rhs);
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

    Ptr<Node> node_;
};

template<typename A>
const typename List<A>::Node::Priv List<A>::Node::priv_{};

}  // namespace gungnir

#endif  // GUNGNIR_LIST_HPP

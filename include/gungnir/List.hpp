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

public:
    List() noexcept : node_(Node::create())
    {}

    List(A x) noexcept
        : node_(Node::create(
                    std::make_shared<A>(std::move(x)),
                    Node::create()))
    {}

    List(A head, List tail) noexcept
        : node_(Node::create(
                    std::make_shared<A>(std::move(head)),
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

    List(std::shared_ptr<const Node> node) noexcept : node_(std::move(node))
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
        foreachImpl(std::move(f));
    }

    template<typename Fn, typename B = Decay<Ret<Fn, A>>>
    List<B> map(Fn f) const
    {
        const auto ff = std::bind(std::move(f), std::placeholders::_1);

        std::vector<std::shared_ptr<const B>> buf;
        buf.reserve(size());

        foreachImpl([&buf, &ff](const A &x) {
            buf.emplace_back(std::make_shared<B>(ff(x)));
        });

        using BN = typename List<B>::Node;
        std::shared_ptr<const BN> hd = BN::create();
        for (auto it = buf.rbegin(); it != buf.rend(); ++it) {
            hd = BN::create(std::move(*it), std::move(hd));
        }

        return hd;
    }

    const A & operator[](std::size_t i) const
    {
        if (i >= size()) {
            throw std::out_of_range("index out of range");
        }
        auto ptr = node_.get();
        for (; i > 0; ptr = ptr->tail.get(), --i) {}
        return *(ptr->head);
    }

private:
    template<typename>
    friend class List;

    class Node final {
    private:
        static class Priv final {} priv_;

    public:
        static std::shared_ptr<Node> create()
        {
            return std::make_shared<Node>(priv_);
        }

        static std::shared_ptr<Node> create(std::shared_ptr<const A> head, std::shared_ptr<const Node> tail)
        {
            return std::make_shared<Node>(priv_, std::move(head), std::move(tail));
        }

        Node(const Priv &) noexcept : size(0)
        {}

        Node(const Priv &, std::shared_ptr<const A> head, std::shared_ptr<const Node> tail) noexcept
            : size(1 + tail->size)
            , head(std::move(head))
            , tail(std::move(tail))
        {}

        const std::size_t size;
        const std::shared_ptr<const A> head;
        const std::shared_ptr<const Node> tail;
    };

    template<typename Fn>
    void foreachImpl(Fn f) const
    {
        for (auto n = node_.get(); n->size > 0; n = n->tail.get()) {
            f(*(n->head));
        }
    }

    std::shared_ptr<const Node> node_;
};

}  // namespace gungnir

#endif  // GUNGNIR_LIST_HPP

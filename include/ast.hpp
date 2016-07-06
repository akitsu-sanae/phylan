/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#ifndef PHYLAN_AST_HPP
#define PHYLAN_AST_HPP

#include <iostream>
#include <memory>

namespace ph {

enum class NodeType {
    Plus,
    Mult,
    Print
};

struct Element {
    virtual int value() const = 0;
    static std::shared_ptr<Element> load(const char* filename);
};

template<NodeType type>
struct Node;

template<>
struct Node<NodeType::Plus> final : public Element
{
    explicit Node(std::shared_ptr<Element> const& lhs, std::shared_ptr<Element> const& rhs) :
        lhs(lhs),
        rhs(rhs)
    {}

    int value() const override {
        return lhs->value() + rhs->value();
    }
private:
    std::shared_ptr<Element> lhs;
    std::shared_ptr<Element> rhs;
};

template<>
struct Node<NodeType::Mult> final : public Element
{
    explicit Node(std::shared_ptr<Element> const& lhs, std::shared_ptr<Element> const& rhs) :
        lhs(lhs),
        rhs(rhs)
    {}

    int value() const override {
        return lhs->value() * rhs->value();
    }
private:
    std::shared_ptr<Element> lhs;
    std::shared_ptr<Element> rhs;
};

template<>
struct Node<NodeType::Print> final : public Element
{
    explicit Node(std::shared_ptr<Element> const& val) :
        val(val)
    {}

    int value() const override {
        std::cout << val->value() << std::endl;
        return val->value();
    }
private:
    std::shared_ptr<Element> val;
};

struct Literal : public Element {
    explicit Literal(int n) :
        val(n)
    {}

    int value() const override {
        return val;
    }

    int val;
};


}

#endif

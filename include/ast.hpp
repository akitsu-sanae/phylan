/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#ifndef PHYLAN_AST_HPP
#define PHYLAN_AST_HPP

#include <iostream>
#include <string>
#include <memory>

#include "utility.hpp"

class btCollisionShape;
class btRigidBody;

namespace ph {

struct World;

enum class NodeType {
    Plus,
    Mult,
    Print
};


struct Element {
public:
    explicit Element(ph::Point const&);
    virtual ~Element();
    virtual int value() const = 0;

    virtual void draw() const = 0;
    virtual void update() = 0;

    virtual void regist(ph::World&) = 0;

    btVector3 position() const;
    btRigidBody* body() const { return m_body; }

    static std::shared_ptr<Element> load(std::string const& filename);
    struct invalid_loading_exception {};
protected:
    btCollisionShape* m_shape = nullptr;
    btRigidBody* m_body = nullptr;
};

template<NodeType type>
struct Node;

template<>
struct Node<NodeType::Plus> final : public Element
{
    explicit Node(ph::Point const& pos, std::shared_ptr<Element> const& lhs, std::shared_ptr<Element> const& rhs) :
        Element(pos),
        lhs(lhs),
        rhs(rhs)
    {}

    int value() const override {
        return lhs->value() + rhs->value();
    }

    void draw() const override;
    void update() override;

    void regist(ph::World&) override;
    std::shared_ptr<Element> lhs;
    std::shared_ptr<Element> rhs;
};

template<>
struct Node<NodeType::Mult> final : public Element
{
    explicit Node(ph::Point const& pos, std::shared_ptr<Element> const& lhs, std::shared_ptr<Element> const& rhs) :
        Element(pos),
        lhs(lhs),
        rhs(rhs)
    {}

    int value() const override {
        return lhs->value() * rhs->value();
    }

    void draw() const override;
    void update() override;

    void regist(ph::World&) override;
    std::shared_ptr<Element> lhs;
    std::shared_ptr<Element> rhs;
};

template<>
struct Node<NodeType::Print> final : public Element
{
    explicit Node(ph::Point const& pos, std::shared_ptr<Element> const& val) :
        Element(pos),
        val(val)
    {}

    int value() const override {
        std::cout << val->value() << std::endl;
        return val->value();
    }

    void draw() const override;
    void update() override;

    void regist(ph::World&) override;
    std::shared_ptr<Element> val;
};

struct Literal : public Element {
    explicit Literal(ph::Point const& pos, int n) :
        Element(pos),
        val(n)
    {}

    int value() const override {
        return val;
    }

    void draw() const override;
    void update() override;

    void regist(ph::World&) override;
    int val;
};


}

#endif

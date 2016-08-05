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
    Print,
    If
};


struct Element {
public:
    explicit Element(ph::Point const&);
    virtual ~Element();
    virtual int value() const = 0;

    virtual void draw(Element const*) const = 0;
    virtual void update() = 0;

    virtual void regist(ph::World&) = 0;
    virtual void remove(ph::World&) = 0;
    virtual Element* next() const = 0;
    virtual Element* prev() const = 0;
    Element* parent() const {
        return m_parent;
    }
    void parent(Element* p) {
        m_parent = p;
    }

    btVector3 position() const;
    btRigidBody* body() const { return m_body; }

    static std::unique_ptr<Element> load(std::istream&);
    void save() const;

    struct invalid_loading_exception {};
    struct invalid_execution_exception {};
protected:
    Element* m_parent = nullptr;
    btCollisionShape* m_shape = nullptr;
    btRigidBody* m_body = nullptr;
};

template<NodeType type>
struct Node;

template<>
struct Node<NodeType::Plus> final : public Element
{
    explicit Node(ph::Point const& pos) :
        Element(pos)
    {}

    int value() const override {
        return lhs->value() + rhs->value();
    }

    void draw(Element const*) const override;
    void update() override;

    void regist(ph::World&) override;
    void remove(ph::World&) override;
    Element* next() const override { return lhs.get(); }
    Element* prev() const override { return rhs.get(); }
    std::unique_ptr<Element> lhs;
    std::unique_ptr<Element> rhs;
};

template<>
struct Node<NodeType::Mult> final : public Element
{
    explicit Node(ph::Point const& pos) :
        Element(pos)
    {}

    int value() const override {
        return lhs->value() * rhs->value();
    }

    void draw(Element const*) const override;
    void update() override;

    void regist(ph::World&) override;
    void remove(ph::World&) override;
    Element* next() const override { return lhs.get(); }
    Element* prev() const override { return rhs.get(); }
    std::unique_ptr<Element> lhs;
    std::unique_ptr<Element> rhs;
};

template<>
struct Node<NodeType::Print> final : public Element
{
    explicit Node(ph::Point const& pos) :
        Element(pos)
    {}

    int value() const override {
        std::cout << val->value() << std::endl;
        return val->value();
    }

    void draw(Element const*) const override;
    void update() override;

    void regist(ph::World&) override;
    void remove(ph::World&) override;
    Element* next() const override { return val.get(); }
    Element* prev() const override { return val.get(); }
    std::unique_ptr<Element> val;
};

template<>
struct Node<NodeType::If> final : public Element
{
    using Element::Element;
    int value() const override {
        if (cond->value())
            return true_->value();
        else
            return false_->value();
    }
    void draw(Element const*) const override;
    void update() override;

    void regist(ph::World&) override;
    void remove(ph::World&) override;
    Element* next() const override { return true_.get(); }
    Element* prev() const override { return false_.get(); }
    std::unique_ptr<Element> cond;
    std::unique_ptr<Element> true_;
    std::unique_ptr<Element> false_;
};

struct Literal : public Element {
    explicit Literal(ph::Point const& pos, int n) :
        Element(pos),
        val(n)
    {}

    int value() const override {
        return val;
    }

    void draw(Element const*) const override;
    void update() override;

    void regist(ph::World&) override;
    void remove(ph::World&) override;
    Element* next() const override { return nullptr; }
    Element* prev() const override { return nullptr; }
    int val;
};

struct Undefined : public Element {
    using Element::Element;

    int value() const override {
        throw invalid_execution_exception{};
    }

    void draw(Element const*) const override;
    void update() override {}

    void regist(ph::World&) override;
    void remove(ph::World&) override;
    Element* next() const override { return nullptr; }
    Element* prev() const override { return nullptr; }
};

template<ph::NodeType T>
inline auto node_cast(ph::Element* e) {
    return dynamic_cast<ph::Node<T>*>(e);
}
template<ph::NodeType T>
inline auto node_cast(ph::Element const* e) {
    return dynamic_cast<ph::Node<T> const*>(e);
}

}

#endif

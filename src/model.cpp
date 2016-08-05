/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <GLFW/glfw3.h>
#include "model.hpp"
#include "world.hpp"
#include "ast.hpp"
#include "rope.hpp"

ph::Model::Model(ph::World& world, std::istream& input) :
    m_world(world)
{
    m_ast = ph::Element::load(input);
    m_ast->regist(world);
    m_ropes = ph::Rope::set(m_ast.get(), *world.world_info());
    m_ropes.push_back(std::make_shared<Rope>(*m_ast, *world.world_info()));
    for (auto&& rope : m_ropes)
        rope->regist(world);

    m_selected_element = m_ast.get();
}

ph::Model::~Model() {
    for (auto&& rope : m_ropes)
        rope->remove(m_world);
    m_ast->remove(m_world);
}

void ph::Model::draw() const {
    m_ast->draw(m_selected_element);
    for (auto const& rope : m_ropes)
        rope->draw();
}

void ph::Model::save() const {
    m_ast->save();
}

void ph::Model::move(ph::Model::Move move) {
    Element* tmp = nullptr;
    switch (move) {
    case Move::Next:
        tmp = m_selected_element->next();
        break;
    case Move::Prev:
        tmp = m_selected_element->prev();
        break;
    case Move::Parent:
        tmp = m_selected_element->parent();
        break;
    }
    if (tmp)
        m_selected_element = tmp;
}

void add_element(
    std::unique_ptr<ph::Element>& ast,
    ph::Element* selected,
    std::unique_ptr<ph::Element>& e) {

    if (auto mult = ph::node_cast<ph::NodeType::Mult>(ast.get())) {
        if (mult->lhs.get() == selected) {
            mult->lhs = std::move(e);
            mult->lhs->parent(mult);
        } else
            add_element(mult->lhs, selected, e);
        if (mult->rhs.get() == selected) {
            mult->rhs = std::move(e);
            mult->rhs->parent(mult);
        } else
            add_element(mult->rhs, selected, e);
    } else if (auto plus = ph::node_cast<ph::NodeType::Plus>(ast.get())) {
        if (plus->lhs.get() == selected) {
            plus->lhs = std::move(e);
            plus->lhs->parent(plus);
        } else
            add_element(plus->lhs, selected, e);
        if (plus->rhs.get() == selected) {
            plus->rhs = std::move(e);
            plus->rhs->parent(plus);
        } else
            add_element(plus->rhs, selected, e);
    } else if (auto print = ph::node_cast<ph::NodeType::Print>(ast.get())) {
        if (print->val.get() == selected) {
            print->val = std::move(e);
            print->val->parent(print);
        } else
            add_element(print->val, selected, e);
    } else if (auto if_ = ph::node_cast<ph::NodeType::If>(ast.get())) {
        if (if_->cond.get() == selected) {
            if_->cond = std::move(e);
            if_->cond->parent(if_);
        } else
            add_element(if_->cond, selected, e);
        if (if_->true_.get() == selected) {
            if_->true_ = std::move(e);
            if_->true_->parent(if_);
        } else
            add_element(if_->true_, selected, e);
        if (if_->false_.get() == selected) {
            if_->false_ = std::move(e);
            if_->false_->parent(if_);
        } else
            add_element(if_->false_, selected, e);
    }
}

std::unique_ptr<ph::Element> make_node(ph::Element const* selected) {
    std::string type = "";
    while (true) {
        std::cout << "what type? (plus, mult, print, number)" << std::endl;
        std::cin >> type;
        if (type == "plus")
            break;
        if (type == "mult")
            break;
        if (type == "print")
            break;
        if (type == "if")
            break;
        if (type == "number")
            break;
    }
    std::unique_ptr<ph::Element> element;
    if (type == "plus") {
        auto pos = ph::Point::from_vec(selected->position());
        auto plus = std::make_unique<ph::Node<ph::NodeType::Plus>>(pos);
        plus->lhs = std::make_unique<ph::Undefined>(pos);
        plus->rhs = std::make_unique<ph::Undefined>(pos);
        plus->lhs->parent(plus.get());
        plus->rhs->parent(plus.get());
        element = std::move(plus);
    } else if (type == "mult") {
        auto pos = ph::Point::from_vec(selected->position());
        auto mult = std::make_unique<ph::Node<ph::NodeType::Mult>>(pos);
        mult->lhs = std::make_unique<ph::Undefined>(pos);
        mult->rhs = std::make_unique<ph::Undefined>(pos);
        mult->lhs->parent(mult.get());
        mult->rhs->parent(mult.get());
        element = std::move(mult);
    } else if (type == "if") {
        auto pos = ph::Point::from_vec(selected->position());
        auto if_ = std::make_unique<ph::Node<ph::NodeType::If>>(pos);
        if_->cond = std::make_unique<ph::Undefined>(pos);
        if_->true_ = std::make_unique<ph::Undefined>(pos);
        if_->false_ = std::make_unique<ph::Undefined>(pos);
        if_->cond->parent(if_.get());
        if_->true_->parent(if_.get());
        if_->false_->parent(if_.get());
        element = std::move(if_);
    } else if (type == "print") {
        auto pos = ph::Point::from_vec(selected->position());
        auto print = std::make_unique<ph::Node<ph::NodeType::Print>>(pos);
        print->val = std::make_unique<ph::Undefined>(pos);
        print->val->parent(print.get());
        element = std::move(print);
    } else if (type == "number") {
        std::cout << "value: ";
        int n;
        std::cin >> n;
        element = std::make_unique<ph::Literal>(ph::Point::from_vec(selected->position()), n);
    }
    return element;
}

void ph::Model::edit() {
    if (m_selected_element == m_ast.get()) {
        std::cerr << "you cen not edit the root node!" << std::endl;
        return;
    }
    std::unique_ptr<Element> element;
    if (!dynamic_cast<ph::Undefined*>(m_selected_element))
        element = std::make_unique<Undefined>(Point::from_vec(m_selected_element->position()));
    else
        element = make_node(m_selected_element);

    for (auto&& rope : m_ropes)
        rope->remove(m_world);
    m_ast->remove(m_world);

    // swap element and m_selected_element
    add_element(m_ast, m_selected_element, element);

    m_ropes.clear();
    m_ropes = ph::Rope::set(m_ast.get(), *m_world.world_info());
    m_ropes.push_back(std::make_shared<Rope>(*m_ast, *m_world.world_info()));

    m_ast->regist(m_world);
    for (auto&& rope : m_ropes)
        rope->regist(m_world);

    m_selected_element = m_ast.get();
}


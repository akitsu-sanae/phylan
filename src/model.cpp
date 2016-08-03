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

ph::Model::Model(ph::World& world, std::string const& filename) :
    m_world(world)
{
    m_ast = ph::Element::load(filename);
    m_ast->regist(world);
    m_ropes = ph::Rope::set(m_ast, *world.world_info());
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

static void draw_cursor(ph::Element const* target) {
    if (!target)
        std::abort();
    glPushMatrix();
    auto pos = target->position();
    glTranslated(pos.x(), pos.y(), pos.z());
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 16; i++) {
        double angle = 2.0*3.141592 * i / 16.0;
        glVertex2d(std::cos(angle), std::sin(angle));
    }
    glEnd();
    glPopMatrix();
}

void ph::Model::draw() const {
    m_ast->draw();
    for (auto const& rope : m_ropes)
        rope->draw();
    draw_cursor(m_selected_element);
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

void ph::Model::edit() {
    if (!dynamic_cast<ph::Undefined*>(m_selected_element)) {
        std::cerr << "you can edit only undefined node" << std::endl;
        return;
    }

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
        if (type == "number")
            break;
    }
    std::shared_ptr<ph::Element> element;
    if (type == "plus") {
        auto pos = Point::from_vec(m_selected_element->position());
        auto plus = std::make_shared<ph::Node<ph::NodeType::Plus>>(pos);
        plus->lhs = std::make_shared<ph::Undefined>(pos);
        plus->rhs = std::make_shared<ph::Undefined>(pos);
        element = plus;
    } else if (type == "mult") {
        auto pos = Point::from_vec(m_selected_element->position());
        auto mult = std::make_shared<ph::Node<ph::NodeType::Mult>>(pos);
        mult->lhs = std::make_shared<ph::Undefined>(pos);
        mult->rhs = std::make_shared<ph::Undefined>(pos);
        element = mult;
    } else if (type == "print") {
        auto pos = Point::from_vec(m_selected_element->position());
        auto print = std::make_shared<ph::Node<ph::NodeType::Print>>(pos);
        print->val = std::make_shared<ph::Undefined>(pos);
        element = print;
    }
    for (auto&& rope : m_ropes)
        rope->remove(m_world);
    m_ast->remove(m_world);

    // swap element and m_selected_element

    m_ast->regist(m_world);
    for (auto&& rope : m_ropes)
        rope->regist(m_world);

    m_selected_element = m_ast.get();
}


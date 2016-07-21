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

    m_selected_element = m_ast;
}

ph::Model::~Model() {
    for (auto&& rope : m_ropes)
        rope->remove(m_world);
    m_ast->remove(m_world);
}

static void draw_cursor(std::shared_ptr<ph::Element> const& target) {
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




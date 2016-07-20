/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <BulletSoftBody/btSoftBody.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include "graphics.hpp"
#include "world.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "ast.hpp"
#include "rope.hpp"

ph::World::Objects::Objects(ph::World& world) :
    world(world)
{
    ast = ph::Element::load();
    ast->regist(world);
    ropes = ph::Rope::set(ast, *world.world_info());
    ropes.push_back(std::make_shared<Rope>(*ast, *world.world_info()));
    for (auto&& rope : ropes)
        rope->regist(world);

    m_current_element = ast;
}

ph::World::Objects::~Objects() {
    for (auto&& rope : ropes)
        rope->remove(world);
}

static void draw_cursor(std::shared_ptr<ph::Element> target) {
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

void ph::World::Objects::draw() const {
    ast->draw();
    for (auto const& rope : ropes)
        rope->draw();
    draw_cursor(m_current_element);
}

void ph::World::Objects::save() const {
    ast->save();
}

void ph::World::Objects::next() {
    auto tmp = m_current_element->next();
    if (tmp)
        m_current_element = tmp;
}
void ph::World::Objects::prev() {
    auto tmp = m_current_element->prev();
    if (tmp)
        m_current_element = tmp;
}
void ph::World::Objects::parent() {
    auto tmp = m_current_element->parent();
    if (tmp)
        m_current_element = tmp;
}

ph::World::World() {
    glfwInit();
    m_window = glfwCreateWindow(640, 480, "phylan", nullptr, nullptr);
    glfwMakeContextCurrent(m_window);

    m_keyboard = std::make_shared<Keyboard>(m_window);
    m_mouse = std::make_shared<Mouse>(m_window);

    init_gl();
    init_bullet();
}

ph::World::~World() {
    glfwTerminate();
}

bool ph::World::update() {

    m_camera.update();
    m_dynamics_world->stepSimulation(dt, 1);
    draw();

    glfwPollEvents();
    m_mouse->update();
    m_keyboard->update();

    mouse_event();
    key_event();


    return !glfwWindowShouldClose(m_window);
}

void ph::World::draw() const {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();

    int width, height;
    glfwGetFramebufferSize( m_window, &width, &height );
    glViewport( 0, 0, width, height );
    gluPerspective( 30.0, (double)width / (double)height, 1.0, 100.0 );
    glTranslated( 0.0, 0.0, -2.0 );
    m_camera.look_at();

    static const GLfloat light_position[] = {0.0, 3.0, 5.0, 1.0};
    glLightfv( GL_LIGHT0, GL_POSITION, light_position);

    if (m_objects)
        m_objects->draw();
    glfwSwapBuffers(m_window);
}



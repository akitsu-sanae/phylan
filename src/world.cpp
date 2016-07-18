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
}

ph::World::Objects::~Objects() {
    for (auto&& rope : ropes)
        rope->remove(world);
}

void ph::World::Objects::draw() const {
    ast->draw();
    for (auto const& rope : ropes)
        rope->draw();
}

void ph::World::Objects::save() const {
    ast->save();
}

ph::World::World() {
    glfwInit();
    m_window = glfwCreateWindow(640, 480, "iml", nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
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



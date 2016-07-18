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

ph::World::World() {
    glfwInit();
    m_window = glfwCreateWindow(640, 480, "iml", nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
    init_gl();
    init_bullet();
    m_ast = ph::Element::load("./example/test.ph");
    m_ast->regist(*this);
    m_ropes = ph::Rope::set(m_ast, *m_dynamics_world_info);
    m_ropes.push_back(std::make_shared<Rope>(*m_ast, *m_dynamics_world_info));
    for (auto&& rope : m_ropes)
        rope->regist(*this);
}

ph::World::~World() {
    for (auto&& rope : m_ropes)
        rope->remove(*this);
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

    m_ast->draw();
    for (auto const& rope : m_ropes)
        rope->draw();
    glfwSwapBuffers(m_window);
}



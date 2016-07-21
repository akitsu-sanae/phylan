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
#include "model.hpp"


ph::World::World(std::string const& filename) {
    glfwInit();
    m_window = glfwCreateWindow(640, 480, "phylan", nullptr, nullptr);
    glfwMakeContextCurrent(m_window);

    m_keyboard = std::make_shared<Keyboard>(m_window);
    m_mouse = std::make_shared<Mouse>(m_window);

    init_gl();
    init_bullet();

    if (filename != "")
        m_model = std::make_shared<Model>(*this, filename);
}

ph::World::~World() {
    m_model.reset();
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

    if (m_model)
        m_model->draw();
    glfwSwapBuffers(m_window);
}



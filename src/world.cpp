/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include "world.hpp"
#include "cube.hpp"
#include "ground.hpp"

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

    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        on_mouse_clicked();
        m_mouse_state.is_clicked_prev = true;
    } else {
        m_mouse_state.is_clicked_prev = false;
    }
    glfwGetCursorPos(m_window, &m_mouse_state.x, &m_mouse_state.y);


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

    m_cube->draw();
    glfwSwapBuffers(m_window);
}



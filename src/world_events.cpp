/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <GLFW/glfw3.h>
#include <iostream>
#include "world.hpp"

void ph::World::on_mouse_clicked() {
    double current_x, current_y;
    glfwGetCursorPos(m_window, &current_x, &current_y);
    double dx = current_x - m_mouse_state.x;
    double dy = current_y - m_mouse_state.y;

    if (m_mouse_state.is_left_clicked_prev)
        m_camera.move_angle(dx, dy);
    if (m_mouse_state.is_right_clicked_prev)
        m_camera.move_target(dx, dy);
}


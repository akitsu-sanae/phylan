/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <cmath>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "camera.hpp"

ph::Camera::Camera() :
    target_position{{0.0, 0.0, 0.0}},
    up_direction{{0.0, 1.0, 0.0}}
{}

void ph::Camera::update() {
}

void ph::Camera::look_at() const {
    double origin_x = m_origin.radius * std::sin(m_origin.theta) * std::cos(m_origin.phi);
    double origin_y = m_origin.radius * std::sin(m_origin.theta) * std::sin(m_origin.phi);
    double origin_z = m_origin.radius * std::cos(m_origin.theta);
    gluLookAt(
            origin_x, origin_y, origin_z,
            target_position[0], target_position[1], target_position[2],
            up_direction[0], up_direction[1], up_direction[2]);
}

void ph::Camera::move(double dx, double dy) {
    m_origin.theta -= dx/100.0;
    m_origin.phi += dy/100.0;
}



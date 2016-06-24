/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "camera.hpp"

ph::Camera::Camera() :
    camera_position{{3.0, 4.0, 5.0}},
    target_position{{0.0, 0.0, 0.0}},
    up_direction{{0.0, 1.0, 0.0}}
{}

void ph::Camera::update() {
    gluLookAt(
            camera_position[0], camera_position[1], camera_position[2],
            target_position[0], target_position[1], target_position[2],
            up_direction[0], up_direction[1], up_direction[2]);
}



/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#ifndef PHYLAN_WORLD_HPP
#define PHYLAN_WORLD_HPP

#include <memory>

#include "cube.hpp"
#include "camera.hpp"

struct GLFWwindow;
class btDiscreteDynamicsWorld;

namespace ph {

struct World {
    explicit World();
    ~World();
    bool update();
private:
    void draw() const;
    void init_gl();
    void init_bullet();
    void init_bodies();
    GLFWwindow* m_window;
    std::unique_ptr<Cube> m_cube;
    Camera m_camera;

    std::shared_ptr<btDiscreteDynamicsWorld> m_dynamics_world;
};

}

#endif

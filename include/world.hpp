/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#ifndef PHYLAN_WORLD_HPP
#define PHYLAN_WORLD_HPP

#include <memory>

#include "base.hpp"
#include "camera.hpp"

struct GLFWwindow;
class btDiscreteDynamicsWorld;

namespace ph {

struct Cube;
struct Ground;

struct World {
interface:
    explicit World();
    ~World();
    bool update();
implementions:
    struct MouseState {
        bool is_clicked_prev = false;
        int x;
        int y;
    } m_mouse_state;

    constexpr static double dt = 0.01;

    void draw() const;

    void init_gl();
    void init_bullet();
    void init_bodies();

private slots:
    void on_mouse_clicked(); // slot

members:
    GLFWwindow* m_window;
    std::unique_ptr<Cube> m_cube;
    std::unique_ptr<Ground> m_ground;
    Camera m_camera;

    std::shared_ptr<btDiscreteDynamicsWorld> m_dynamics_world;
};

}

#endif

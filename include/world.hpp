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
class btDynamicsWorld;
class btSoftBodyWorldInfo;

namespace ph {

struct Element;
struct Ground;

struct World {
interface:
    explicit World();
    ~World();
    bool update();

    std::shared_ptr<btDynamicsWorld> dynamics_world() {
        return m_dynamics_world;
    }
implementions:
    struct MouseState {
        bool is_clicked_prev = false;
        double x;
        double y;
    } m_mouse_state;

    constexpr static double dt = 0.01;

    void draw() const;

    void init_gl();
    void init_bullet();
    void init_bodies();
private slots:
    void on_mouse_clicked();

members:
    GLFWwindow* m_window;
    std::unique_ptr<Ground> m_ground;
    std::shared_ptr<Element> m_ast;
    Camera m_camera;

    std::shared_ptr<btDynamicsWorld> m_dynamics_world;
    std::shared_ptr<btSoftBodyWorldInfo> m_dynamics_world_info;
};

}

#endif

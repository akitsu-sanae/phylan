/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#ifndef PHYLAN_WORLD_HPP
#define PHYLAN_WORLD_HPP

#include <memory>
#include <vector>

#include "base.hpp"
#include "camera.hpp"

struct GLFWwindow;
class btDynamicsWorld;
class btSoftRigidDynamicsWorld;
class btSoftBodyWorldInfo;
class btSoftBody;

namespace ph {

struct Element;
struct Rope;

struct World {
interface:
    explicit World();
    ~World();
    bool update();

    std::shared_ptr<btSoftRigidDynamicsWorld> dynamics_world() {
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
    std::shared_ptr<Element> m_ast;
    std::vector<std::shared_ptr<Rope>> m_ropes;
    Camera m_camera;

    std::shared_ptr<btSoftRigidDynamicsWorld> m_dynamics_world;
    std::shared_ptr<btSoftBodyWorldInfo> m_dynamics_world_info;
};

}

#endif

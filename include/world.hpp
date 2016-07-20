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
struct btSoftBodyWorldInfo;
class btSoftBody;

namespace ph {

struct Element;
struct Rope;
struct Model;

struct Keyboard;
struct Mouse;

struct World {
interface:
    explicit World(std::string const& filename = "");
    ~World();
    bool update();

    std::shared_ptr<btSoftRigidDynamicsWorld> dynamics_world() {
        return m_dynamics_world;
    }
    std::shared_ptr<btSoftBodyWorldInfo> world_info() {
        return m_dynamics_world_info;
    }
implementions:
    std::shared_ptr<Keyboard> m_keyboard;
    std::shared_ptr<Mouse> m_mouse;

    constexpr static double dt = 0.01;

    void draw() const;

    void init_gl();
    void init_bullet();
    void init_bodies();
private slots:
    void mouse_event();

    void key_event();

members:
    GLFWwindow* m_window;
    Camera m_camera;

    std::shared_ptr<Model> m_model;

    std::shared_ptr<btSoftRigidDynamicsWorld> m_dynamics_world;
    std::shared_ptr<btSoftBodyWorldInfo> m_dynamics_world_info;
};

}

#endif

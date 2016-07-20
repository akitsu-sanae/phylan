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

struct Keyboard;

struct World {
interface:
    explicit World();
    ~World();
    bool update();

    std::shared_ptr<btSoftRigidDynamicsWorld> dynamics_world() {
        return m_dynamics_world;
    }
    std::shared_ptr<btSoftBodyWorldInfo> world_info() {
        return m_dynamics_world_info;
    }
implementions:
    struct MouseState {
        bool is_left_clicked_prev = false;
        bool is_right_clicked_prev = false;
        double x;
        double y;
    } m_mouse_state;

    std::shared_ptr<Keyboard> m_keyboard;

    constexpr static double dt = 0.01;

    void draw() const;

    void init_gl();
    void init_bullet();
    void init_bodies();
private slots:
    void mouse_event();
    void on_mouse_clicked();

    void key_event();

members:
    GLFWwindow* m_window;
    Camera m_camera;

    struct Objects {
        explicit Objects(ph::World&);
        ~Objects();
        void draw() const;
        void save() const;
        std::shared_ptr<Objects> load();

        void next();
        void prev();
        void parent();
    private:
        ph::World& world;
        std::shared_ptr<Element> ast;
        std::vector<std::shared_ptr<Rope>> ropes;
        std::shared_ptr<Element> m_current_element;
    };

    std::shared_ptr<Objects> m_objects;

    std::shared_ptr<btSoftRigidDynamicsWorld> m_dynamics_world;
    std::shared_ptr<btSoftBodyWorldInfo> m_dynamics_world_info;
};

}

#endif

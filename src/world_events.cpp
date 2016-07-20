/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <GLFW/glfw3.h>
#include <iostream>
#include "world.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "ast.hpp"

void ph::World::mouse_event() {
    if (m_mouse->state(Mouse::Button::Left) != Mouse::State::Release)
        m_camera.move_angle(m_mouse->diff()[0], m_mouse->diff()[1]);
    if (m_mouse->state(Mouse::Button::Right) != Mouse::State::Release)
        m_camera.move_target(m_mouse->diff()[0], m_mouse->diff()[1]);
}

void ph::World::key_event() {
    if (m_keyboard->state(Key::Z) == KeyState::Push)
        m_objects->save();
    if (m_keyboard->state(Key::X) == KeyState::Push)
        m_objects = std::make_shared<Objects>(*this);

    if (m_keyboard->state(Key::J) == KeyState::Push)
        m_objects->next();
    if (m_keyboard->state(Key::L) == KeyState::Push)
        m_objects->prev();
    if (m_keyboard->state(Key::I) == KeyState::Push)
        m_objects->parent();

}


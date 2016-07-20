/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <GLFW/glfw3.h>
#include "keyboard.hpp"

static int convert_key(ph::Key key) {
    switch (key) {
    case ph::Key::A:
        return GLFW_KEY_A;
    case ph::Key::B:
        return GLFW_KEY_B;
    case ph::Key::C:
        return GLFW_KEY_C;
    case ph::Key::D:
        return GLFW_KEY_D;
    case ph::Key::E:
        return GLFW_KEY_E;
    case ph::Key::F:
        return GLFW_KEY_F;
    case ph::Key::G:
        return GLFW_KEY_G;
    case ph::Key::H:
        return GLFW_KEY_H;
    case ph::Key::I:
        return GLFW_KEY_I;
    case ph::Key::J:
        return GLFW_KEY_J;
    case ph::Key::K:
        return GLFW_KEY_K;
    case ph::Key::L:
        return GLFW_KEY_L;
    case ph::Key::M:
        return GLFW_KEY_M;
    case ph::Key::N:
        return GLFW_KEY_N;
    case ph::Key::O:
        return GLFW_KEY_O;
    case ph::Key::P:
        return GLFW_KEY_P;
    case ph::Key::Q:
        return GLFW_KEY_Q;
    case ph::Key::R:
        return GLFW_KEY_R;
    case ph::Key::S:
        return GLFW_KEY_S;
    case ph::Key::T:
        return GLFW_KEY_T;
    case ph::Key::U:
        return GLFW_KEY_U;
    case ph::Key::V:
        return GLFW_KEY_V;
    case ph::Key::W:
        return GLFW_KEY_W;
    case ph::Key::X:
        return GLFW_KEY_X;
    case ph::Key::Y:
        return GLFW_KEY_Y;
    case ph::Key::Z:
        return GLFW_KEY_Z;
    }
}

static ph::Key convert_key(int key) {
    switch (key) {
    case GLFW_KEY_A:
        return ph::Key::A;
    case GLFW_KEY_B:
        return ph::Key::B;
    case GLFW_KEY_C:
        return ph::Key::C;
    case GLFW_KEY_D:
        return ph::Key::D;
    case GLFW_KEY_E:
        return ph::Key::E;
    case GLFW_KEY_F:
        return ph::Key::F;
    case GLFW_KEY_G:
        return ph::Key::G;
    case GLFW_KEY_H:
        return ph::Key::H;
    case GLFW_KEY_I:
        return ph::Key::I;
    case GLFW_KEY_J:
        return ph::Key::J;
    case GLFW_KEY_K:
        return ph::Key::K;
    case GLFW_KEY_L:
        return ph::Key::L;
    case GLFW_KEY_M:
        return ph::Key::M;
    case GLFW_KEY_N:
        return ph::Key::N;
    case GLFW_KEY_O:
        return ph::Key::O;
    case GLFW_KEY_P:
        return ph::Key::P;
    case GLFW_KEY_Q:
        return ph::Key::Q;
    case GLFW_KEY_R:
        return ph::Key::R;
    case GLFW_KEY_S:
        return ph::Key::S;
    case GLFW_KEY_T:
        return ph::Key::T;
    case GLFW_KEY_U:
        return ph::Key::U;
    case GLFW_KEY_V:
        return ph::Key::V;
    case GLFW_KEY_W:
        return ph::Key::W;
    case GLFW_KEY_X:
        return ph::Key::X;
    case GLFW_KEY_Y:
        return ph::Key::Y;
    case GLFW_KEY_Z:
        return ph::Key::Z;
    }
}

void ph::Keyboard::update() {
    auto update = [this](Key key) {
        bool is_pressed = glfwGetKey(m_window, convert_key(key)) == GLFW_PRESS;
        if (!is_pressed)
            m_key_states[key] = KeyState::Release;
        else if (m_key_states[key] == KeyState::Release)
            m_key_states[key] = KeyState::Push;
        else
            m_key_states[key] = KeyState::Hold;
    };
    for (int i = 0; i <= static_cast<int>(Key::Space); i++)
        update(static_cast<Key>(i));
}

ph::KeyState ph::Keyboard::state(ph::Key key) const {
    auto it = m_key_states.find(key);
    if (it == m_key_states.end())
        throw unknown_key_exception{};
    return it->second; 
}

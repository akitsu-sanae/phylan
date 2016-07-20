/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <GLFW/glfw3.h>
#include "mouse.hpp"

static int convert_button(ph::Mouse::Button button) {
    switch (button) {
    case ph::Mouse::Button::Left:
        return GLFW_MOUSE_BUTTON_1;
    case ph::Mouse::Button::Right:
        return GLFW_MOUSE_BUTTON_2;
    }
}

void ph::Mouse::update() {

    // button

    auto update = [this](Button button) {
        bool is_pressed = glfwGetMouseButton(m_window, convert_button(button));
        if (!is_pressed)
            m_button_state[button] = State::Release;
        else if (m_button_state[button] == State::Release)
            m_button_state[button] = State::Push;
        else
            m_button_state[button] = State::Hold;
    };
    update(Button::Left);
    update(Button::Right);

    // cursor
    m_prev_cursor_position = m_cursor_position;
    glfwGetCursorPos(
        m_window,
        &m_cursor_position.at(0),
        &m_cursor_position.at(1));
}

ph::Mouse::State ph::Mouse::state(ph::Mouse::Button button) const {
    auto it = m_button_state.find(button);
    if (it == m_button_state.end())
        throw unknown_button_exception{};
    return it->second;
}

/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#ifndef PHYLAN_MOUSE_HPP
#define PHYLAN_MOUSE_HPP

#include <unordered_map>
#include <array>

struct GLFWwindow;

namespace ph {

struct Mouse {
    enum class Button {
        Left,
        Right
    };
    enum class State {
        Release,
        Push,
        Hold
    };

    struct unknown_button_exception {};

    explicit Mouse(GLFWwindow* win) :
        m_window(win)
    {}

    void update();
    State state(Button) const;
    double x() const { return m_cursor_position.at(0); }
    double y() const { return m_cursor_position.at(1); }
    std::array<double, 2> diff() const {
        return std::array<double, 2>{
            m_cursor_position.at(0) - m_prev_cursor_position.at(0),
            m_cursor_position.at(1) - m_prev_cursor_position.at(1)
        };
    }
private:
    std::unordered_map<Button, State> m_button_state;
    GLFWwindow* m_window;
    std::array<double, 2> m_cursor_position;
    std::array<double, 2> m_prev_cursor_position;
};

}

#endif

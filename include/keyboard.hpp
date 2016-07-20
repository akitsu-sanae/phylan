/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#ifndef PHYLAN_KEYBOARD_HPP
#define PHYLAN_KEYBOARD_HPP

#include <unordered_map>

struct GLFWwindow;

namespace ph {

enum class Key {
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    Ctrl,
    Shift,
    Enter,
    Space
};

enum class KeyState {
    Release,
    Push,
    Hold
};

struct Keyboard {
    struct unknown_key_exception : std::exception {};

    explicit Keyboard(GLFWwindow* win) :
        m_window(win)
    {}
    void update();
    KeyState state(Key) const;

private:
    std::unordered_map<Key, KeyState> m_key_states;
    GLFWwindow* m_window;
};

}


#endif

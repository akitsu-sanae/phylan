/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "world.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "model.hpp"
#include "ast.hpp"

void ph::World::mouse_event() {
    if (m_mouse->state(Mouse::Button::Left) != Mouse::State::Release)
        m_camera.move_angle(m_mouse->diff()[0], m_mouse->diff()[1]);
    if (m_mouse->state(Mouse::Button::Right) != Mouse::State::Release)
        m_camera.move_target(m_mouse->diff()[0], m_mouse->diff()[1]);
}

static void command_mode(std::shared_ptr<ph::Model>& model, ph::World& world) {
    std::string command;
    while (true) {
        std::cout << "command mode" << std::endl;
        std::cout << "    load: import phylan program file(.ph)" << std::endl;
        std::cout << "    save: export phylan program file(.ph)" << std::endl;
        std::cout << "    edit: edit current node" << std::endl;
        std::cin >> command;

        if (command == "load" || command == "save" || command == "edit")
            break;
    }
    if (command == "load") {
        std::cout << "import filename: ";
        std::string filename;
        std::cin >> filename;
        std::ifstream input(filename);
        if (input.fail()) {
            std::cerr << "can not open file: " << filename << std::endl;
            return;
        }
        model = std::make_shared<ph::Model>(world, input);
    } else if (command == "save") {
        model->save();
    } else if (command == "edit") {
        model->edit();
    }

}

void ph::World::key_event() {
    if (m_keyboard->state(Key::C) == KeyState::Push)
        command_mode(m_model, *this);

    if (m_keyboard->state(Key::V) == KeyState::Push) {
        try {
            std::cout << m_model->eval() << std::endl;
        } catch (Element::invalid_execution_exception) {
            std::cerr << "Undefined Node can not be calculated" << std::endl;
        }
    }

    if (m_keyboard->state(Key::J) == KeyState::Push)
        m_model->move(Model::Move::Next);
    if (m_keyboard->state(Key::L) == KeyState::Push)
        m_model->move(Model::Move::Prev);
    if (m_keyboard->state(Key::I) == KeyState::Push)
        m_model->move(Model::Move::Parent);
}


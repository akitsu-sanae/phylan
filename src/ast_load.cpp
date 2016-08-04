/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <picojson.h>
#include <fstream>
#include <iostream>

#include "ast.hpp"

static std::unique_ptr<ph::Element> read(picojson::value const& v, ph::Point const& current_position, ph::Element* parent) {
    auto next_position = ph::Point{
        current_position.x + double(std::rand() % 1000) / 100000.0,
        current_position.y - 0.5,
        current_position.z + double(std::rand() % 1000) / 100000.0
    };
    std::unique_ptr<ph::Element> element;
    if (v.is<double>())
        element = std::make_unique<ph::Literal>(current_position, static_cast<int>(v.get<double>()));
    else if (v.is<picojson::object>()){
        auto& obj = v.get<picojson::object>();
        auto op = obj.find("op")->second.get<std::string>();
        if (op == "plus") {
            auto plus = std::make_unique<ph::Node<ph::NodeType::Plus>>(current_position);
            plus->lhs = read(obj.at("lhs"), next_position, plus.get());
            plus->rhs = read(obj.at("rhs"), next_position, plus.get());
            element = std::move(plus);
        } else if (op == "mult") {
            auto mult = std::make_unique<ph::Node<ph::NodeType::Mult>>(current_position);
            mult->lhs = read(obj.at("lhs"), next_position, mult.get());
            mult->rhs = read(obj.at("rhs"), next_position, mult.get());
            element = std::move(mult);
        } else if (op == "print") {
            auto print = std::make_unique<ph::Node<ph::NodeType::Print>>(current_position);
            print->val = read(obj.at("val"), next_position, print.get());
            element = std::move(print);
        } else if (op == "undefined") {
            element = std::make_unique<ph::Undefined>(current_position);
        } else {
            throw ph::Element::invalid_loading_exception{};
        }
    } else
        throw ph::Element::invalid_loading_exception{};
    element->parent(parent);
    return element;
}

std::unique_ptr<ph::Element> ph::Element::load(std::string const& filename) {
    std::ifstream input(filename);
    if (input.fail())
        return nullptr;

    picojson::value v;
    input >> v;
    return read(v, ph::Point{ 0.0, 10.0, 0.0 }, nullptr);
}

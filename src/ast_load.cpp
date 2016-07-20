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

static std::shared_ptr<ph::Element> read(picojson::value const& v, ph::Point const& current_position) {
    auto next_position = ph::Point{
        current_position.x + double(std::rand() % 1000) / 10000.0,
        current_position.y - 0.5,
        current_position.z + double(std::rand() % 1000) / 10000.0
    };
    if (v.is<double>())
        return std::make_shared<ph::Literal>(current_position, static_cast<int>(v.get<double>()));
    if (!v.is<picojson::object>())
        return std::make_shared<ph::Literal>(ph::Point{}, 42);
    auto& obj = v.get<picojson::object>();
    auto op = obj.find("op")->second.get<std::string>();
    if (op == "plus") {
        return std::make_shared<ph::Node<ph::NodeType::Plus>>(
            current_position,
            read(obj.at("lhs"), next_position),
            read(obj.at("rhs"), next_position)
            );
    } if (op == "mult") {
        return std::make_shared<ph::Node<ph::NodeType::Mult>>(
            current_position,
            read(obj.at("lhs"), next_position),
            read(obj.at("rhs"), next_position)
            );
    } if (op == "print") {
        return std::make_shared<ph::Node<ph::NodeType::Print>>(
            current_position,
            read(obj.at("val"), next_position)
            );
    } else {
        throw ph::Element::invalid_loading_exception{};
    }
}

std::shared_ptr<ph::Element> ph::Element::load(std::string const& filename) {
    std::ifstream input(filename);
    if (input.fail())
        return nullptr;

    picojson::value v;
    input >> v;
    return read(v, ph::Point{ 0.0, 10.0, 0.0 });
}

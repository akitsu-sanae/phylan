/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#include <fstream>
#include <iostream>
#include <string>

#include <picojson.h>
#include "ast.hpp"

template<ph::NodeType T>
auto node_cast(ph::Element const* e) {
    return dynamic_cast<ph::Node<T> const*>(e);
}

picojson::value ast_to_json(ph::Element const* e) {
    if (auto plus = node_cast<ph::NodeType::Plus>(e)) {
        picojson::object obj;
        obj["op"] = picojson::value("plus");
        obj["lhs"] = ast_to_json(plus->lhs.get());
        obj["rhs"] = ast_to_json(plus->rhs.get());
        return picojson::value(obj);
    } else if (auto mult = node_cast<ph::NodeType::Mult>(e)) {
        picojson::object obj;
        obj["op"] = picojson::value("mult");
        obj["lhs"] = ast_to_json(mult->lhs.get());
        obj["rhs"] = ast_to_json(mult->rhs.get());
        return picojson::value(obj);
    } else if (auto print = node_cast<ph::NodeType::Print>(e)) {
        picojson::object obj;
        obj["op"] = picojson::value("print");
        obj["val"] = ast_to_json(print->val.get());
        return picojson::value(obj);
    } else if (auto lit = dynamic_cast<ph::Literal const*>(e)) {
        return picojson::value((double)lit->val);
    }
}

void ph::Element::save() const {
    std::string filename;
    std::cout << "filename? ";
    std::cin >> filename;
    std::ofstream output(filename);

    if (!output) {
        std::cerr << "can not open file: " << filename << std::endl;
        return;
    }
    output << ast_to_json(this).serialize();
}

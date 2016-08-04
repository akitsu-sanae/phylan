/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#ifndef PHYLAN_MODEL_HPP
#define PHYLAN_MODEL_HPP

#include <memory>
#include <vector>

namespace ph {

struct World;
struct Element;
struct Rope;

struct Model {
    explicit Model(World&, std::string const&);
    ~Model();

    void draw() const;
    void save() const;

    enum class Move {
        Next,
        Prev,
        Parent
    };
    void move(Move);
    void edit();
private:
    World& m_world;

    std::unique_ptr<Element> m_ast;
    std::vector<std::shared_ptr<Rope>> m_ropes;
    Element* m_selected_element;
};

}

#endif

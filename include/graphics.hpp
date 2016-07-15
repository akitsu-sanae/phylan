/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#ifndef PHYLAN_GRAPHICS_HPP
#define PHYLAN_GRAPHICS_HPP

#include "utility.hpp"

class btSoftBody;

namespace ph {
namespace graphics {

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct Edge {
    Point nodes[2];
};

struct Face {
    Point nodes[3];
    Face(Point const& n1, Point const& n2, Point const& n3) :
        nodes{ n1, n2, n3 }
    {}
};

void draw_sphere(Point const&, Color const&);
void draw_rope(btSoftBody const&);

}
}

#endif

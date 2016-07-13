/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <vector>
#include <cmath>
#include <GLFW/glfw3.h>
#include "graphics.hpp"

using Face = ph::graphics::Face;
using Color = ph::graphics::Color;

static std::vector<Face> create_sphere_faces() {
    auto mapped_position = [](int x, int y) -> ph::Point {
        double phi = 2.0 * 3.141592 * x / 16.0;
        double theta = 2.0 * 3.141592 * y / 16.0;
        return ph::Point {
            std::sin(theta)*std::cos(phi),
            std::sin(theta)*std::sin(phi),
            std::cos(theta)
        };
    };
    std::vector<Face> result;
    result.reserve(16 * 16 * 2);
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            auto left_top = mapped_position(x, y);
            auto left_bottom = mapped_position(x, y + 1);
            auto right_top = mapped_position(x + 1, y);
            auto right_bottom = mapped_position(x + 1, y + 1);
            result.emplace_back(left_top, right_top, left_bottom);
            result.emplace_back(right_bottom, left_bottom, right_top);
        }
    }
    return result;
}

void ph::graphics::draw_sphere(Point const& pos, Color const& color) {
    auto faces = create_sphere_faces();
    glPushMatrix();
    glTranslated(pos.x, pos.y, pos.z);
    glBegin(GL_TRIANGLES);
    glColor3ub(color.r, color.g, color.b);
    for (auto const& face : faces) {
        for (auto const& node : face.nodes) {
            glVertex3d(node.x, node.y, node.z);
        }
    }
    glEnd();
    glPopMatrix();
}

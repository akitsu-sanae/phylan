/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <vector>
#include <array>
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
    static auto faces = create_sphere_faces();
    glPushMatrix();
    glTranslated(pos.x, pos.y, pos.z);

    std::array<GLfloat, 3> v = {{
        static_cast<GLfloat>(color.r),
        static_cast<GLfloat>(color.g),
        static_cast<GLfloat>(color.b)
    }};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, v.data());
    glBegin(GL_TRIANGLES);
    for (auto const& face : faces) {
        for (auto const& node : face.nodes) {
            glVertex3d(node.x, node.y, node.z);
        }
    }
    glEnd();
    glPopMatrix();
}

#include <BulletSoftBody/btSoftBody.h>
#include <iostream>

void ph::graphics::draw_rope(btSoftBody const& body) {
    if (body.m_faces.size() != 0) {
        std::cerr << "invalid soft body" << std::endl;
        std::abort();
    }

    std::array<GLfloat, 3> v = {{ 255, 255, 255}};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, v.data());
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < body.m_nodes.size(); i++) {
        btSoftBody::Node const& node = body.m_nodes.at(i);
        glVertex3f(node.m_x.x(), node.m_x.y(), node.m_x.z());
    }
    glEnd();
}

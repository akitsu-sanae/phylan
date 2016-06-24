/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <GLFW/glfw3.h>
#include "ground.hpp"
#include "cube.hpp"

static btRigidBody* create_rigid_body(
        double mass,
        btTransform const& trans,
        btCollisionShape* shape,
        int index) {
    bool is_dynamic = mass != 0.0;
    btVector3 inertia(0, 0, 0);
    if (is_dynamic)
        shape->calculateLocalInertia(mass, inertia);
    auto motion_state = new btDefaultMotionState(trans);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion_state, shape, inertia);
    auto body = new btRigidBody(info);
    body->setUserIndex(index);
    return body;
}

ph::Ground::Ground(btDiscreteDynamicsWorld& world) {
    btTransform trans;
    trans.setIdentity();

    m_shape = new btBoxShape(
            btVector3(20, Cube::cube_half_length, 20));
    trans.setOrigin(btVector3(0, height - Cube::cube_half_length, 0));

    m_body = create_rigid_body(0.0, trans, m_shape, 0);
    world.addRigidBody(m_body);
}

ph::Ground::~Ground() {
    if (m_body->getMotionState())
        delete m_body->getMotionState();
    delete m_shape;
    delete m_body;
}

void ph::Ground::draw() const {
    ;
}



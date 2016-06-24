/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#include <LinearMath/btVector3.h>
#include "world.hpp"

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


void ph::World::init_bodies() {
    btTransform trans;
    trans.setIdentity();

    const btScalar cube_half_length = 0.2;
    const btScalar ground_height = 0.0;

    auto ground_shape = new btBoxShape(
            btVector3(20, cube_half_length, 20));
    trans.setOrigin(btVector3(0, ground_height - cube_half_length, 0));

    auto ground_body = create_rigid_body(0.0, trans, ground_shape, 0);
    m_dynamics_world->addRigidBody(ground_body);

    m_cube = std::make_unique<Cube>(*m_dynamics_world);
}


void ph::World::init_bullet() {
    auto config = new btDefaultCollisionConfiguration();
    auto dispatcher = new btCollisionDispatcher(config);
    auto overlapping_pair_cache = new btDbvtBroadphase();
    auto solver = new btSequentialImpulseConstraintSolver();
    m_dynamics_world = std::make_shared<btDiscreteDynamicsWorld>(dispatcher, overlapping_pair_cache, solver, config);
    m_dynamics_world->setGravity(btVector3(0, -9.8, 0));

    init_bodies();
}

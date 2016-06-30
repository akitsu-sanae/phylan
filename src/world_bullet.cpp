/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#include <LinearMath/btVector3.h>
#include "world.hpp"
#include "cube.hpp"
#include "ground.hpp"

void ph::World::init_bodies() {
    m_ground = std::make_unique<Ground>(*m_dynamics_world);
    m_cube = std::make_unique<Cube>(*m_dynamics_world);
}


void ph::World::init_bullet() {
    auto config = new btDefaultCollisionConfiguration();
    auto dispatcher = new btCollisionDispatcher(config);
    auto overlapping_pair_cache = new btDbvtBroadphase();
    auto solver = new btSequentialImpulseConstraintSolver();
    m_dynamics_world = std::make_shared<btDiscreteDynamicsWorld>(dispatcher, overlapping_pair_cache, solver, config);
    m_dynamics_world->setGravity(btVector3(0, -0.4, 0));

    init_bodies();
}

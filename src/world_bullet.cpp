/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#include <LinearMath/btVector3.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody\btSoftBody.h>

#include "world.hpp"
#include "cube.hpp"
#include "ground.hpp"

void ph::World::init_bodies() {
    m_ground = std::make_unique<Ground>(*m_dynamics_world);
    m_cube = std::make_unique<Cube>(*m_dynamics_world);
}


void ph::World::init_bullet() {
    auto config = new btSoftBodyRigidBodyCollisionConfiguration();
    auto dispatcher = new btCollisionDispatcher(config);
    auto overlapping_pair_cache = new btDbvtBroadphase();
    auto solver = new btSequentialImpulseConstraintSolver();
    m_dynamics_world = std::make_shared<btSoftRigidDynamicsWorld>(dispatcher, overlapping_pair_cache, solver, config);
    m_dynamics_world->setGravity(btVector3(0, -9.8, 0));

    m_dynamics_world_info = std::make_shared<btSoftBodyWorldInfo>();
    m_dynamics_world_info->m_dispatcher = dispatcher;
    m_dynamics_world_info->m_broadphase = overlapping_pair_cache;
    m_dynamics_world_info->m_sparsesdf.Initialize();
    m_dynamics_world_info->m_gravity.setValue(0, -9.8, 0);
    m_dynamics_world_info->air_density = 1.2;

    init_bodies();
}

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

static const GLfloat cube_material[] = { 0.2, 0.6, 0.4, 1.0 };
static const GLdouble cube_normal[][3] = {
  { 0.0, 0.0,-1.0 },
  { 1.0, 0.0, 0.0 },
  { 0.0, 0.0, 1.0 },
  {-1.0, 0.0, 0.0 },
  { 0.0,-1.0, 0.0 },
  { 0.0, 1.0, 0.0 }
};
static const int cube_face[][4] = {
    { 0, 1, 2, 3 },
    { 1, 5, 6, 2 },
    { 5, 4, 7, 6 },
    { 4, 0, 3, 7 },
    { 4, 5, 1, 0 },
    { 3, 2, 6, 7 }
};
static const GLdouble cube_vertex[][3] = {
    { 0.0, 0.0, 0.0 },
    { 1.0, 0.0, 0.0 },
    { 1.0, 1.0, 0.0 },
    { 0.0, 1.0, 0.0 },
    { 0.0, 0.0, 1.0 },
    { 1.0, 0.0, 1.0 },
    { 1.0, 1.0, 1.0 },
    { 0.0, 1.0, 1.0 }
};

ph::Cube::Cube(btDiscreteDynamicsWorld& world) {
    m_shape = new btBoxShape(
            btVector3(cube_half_length, cube_half_length, cube_half_length));
    btQuaternion qrot (0, 0, 0, 1);
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3(0, Ground::height + 10.0 + 10.0 * cube_half_length, 0));
    trans.setRotation(qrot);

    m_body = create_rigid_body(1.0, trans, m_shape, 0);
    world.addRigidBody(m_body);

    m_body->setCcdMotionThreshold(cube_half_length);
    m_body->setCcdSweptSphereRadius(0.005 * cube_half_length);
}

ph::Cube::~Cube() {
    if (m_body->getMotionState())
        delete m_body->getMotionState();
    delete m_shape;
    delete m_body;
}

void ph::Cube::draw() const {
    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);

    glPushMatrix();
    glTranslated(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cube_material);
    glBegin( GL_QUADS );
    for (size_t i = 0; i < 6; ++i) {
		glNormal3dv(cube_normal[i]);
        for (size_t j = 0; j < 4; ++j)
            glVertex3dv( cube_vertex[ cube_face[i][j] ] );
    }
    glEnd();

    glPopMatrix();
}


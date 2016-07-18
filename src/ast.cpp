/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include "ast.hpp"
#include "world.hpp"
#include "graphics.hpp"

btVector3 ph::Element::position() const {
    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);
    return trans.getOrigin();
}


static btRigidBody* create_rigid_body(btTransform const& trans, btCollisionShape* shape)
{
    btVector3 inertia(0, 0, 0);
    double mass = 1.0;
    shape->calculateLocalInertia(mass, inertia);

    auto motion_state = new btDefaultMotionState(trans);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion_state, shape, inertia);
    auto body = new btRigidBody(info);
    body->setUserIndex(0);
    return body;
}


ph::Element::Element(ph::Point const& position) {
    m_shape = new btSphereShape(1.0);
    btQuaternion qrot(0, 0, 0, 1);
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3(position.x, position.y, position.z));
    trans.setRotation(qrot);

    m_body = create_rigid_body(trans, m_shape);
    m_body->setCcdMotionThreshold(1.0);
    m_body->setCcdSweptSphereRadius(0.05);
}

ph::Element::~Element() {
    if (m_body->getMotionState())
        delete m_body->getMotionState();
    delete m_shape;
    delete m_body;
}

void ph::Node<ph::NodeType::Plus>::draw() const {
    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);
    ph::graphics::draw_sphere(
        ph::Point::from_trans(trans),
        ph::graphics::Color{0, 0, 255});
    lhs->draw();
    rhs->draw();
}

void ph::Node<ph::NodeType::Plus>::update() {
    lhs->update();
    rhs->update();
}

void ph::Node<ph::NodeType::Plus>::regist(ph::World& world) {
    world.dynamics_world()->addRigidBody(m_body);
    lhs->regist(world);
    rhs->regist(world);
}

void ph::Node<ph::NodeType::Mult>::draw() const {
    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);
    ph::graphics::draw_sphere(
        ph::Point::from_trans(trans),
        ph::graphics::Color{ 0, 255, 0 });
    lhs->draw();
    rhs->draw();
}

void ph::Node<ph::NodeType::Mult>::regist(ph::World& world) {
    world.dynamics_world()->addRigidBody(m_body);
    lhs->regist(world);
    rhs->regist(world);
}

void ph::Node<ph::NodeType::Mult>::update() {
    lhs->update();
    rhs->update();
}

void ph::Node<ph::NodeType::Print>::draw() const {
    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);
    ph::graphics::draw_sphere(
        ph::Point::from_trans(trans),
        ph::graphics::Color{ 255, 0, 0 });
    val->draw();
}

void ph::Node<ph::NodeType::Print>::regist(ph::World& world) {
    world.dynamics_world()->addRigidBody(m_body);
    val->regist(world);
}

void ph::Node<ph::NodeType::Print>::update() {
    val->update();
}

void ph::Literal::draw() const {
    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);
    ph::graphics::draw_sphere(
        ph::Point::from_trans(trans),
        ph::graphics::Color{ 255, 255, 255 });
}

void ph::Literal::update() {
}

void ph::Literal::regist(ph::World& world) {
    world.dynamics_world()->addRigidBody(m_body);
}




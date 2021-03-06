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
    float mass = 1.0;
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
    m_body->setCcdSweptSphereRadius(0.05f);
}

ph::Element::~Element() {
    if (m_body->getMotionState())
        delete m_body->getMotionState();
    delete m_shape;
    delete m_body;
}

void ph::Node<ph::NodeType::Plus>::draw(ph::Element const* selected) const {
    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);
    ph::graphics::Color color = {40, 190, 60};
    if (this == selected)
        color = color + ph::graphics::Color{40, 40, 40};
    ph::graphics::draw_sphere(
        ph::Point::from_trans(trans),
        color);
    lhs->draw(selected);
    rhs->draw(selected);
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

void ph::Node<ph::NodeType::Plus>::remove(ph::World& world) {
    world.dynamics_world()->removeRigidBody(m_body);
    lhs->remove(world);
    rhs->remove(world);
}

void ph::Node<ph::NodeType::Mult>::draw(ph::Element const* selected) const {
    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);
    ph::graphics::Color color = { 80, 100, 210 };
    if (this == selected)
        color = color + ph::graphics::Color{ 40, 40, 40 };
    ph::graphics::draw_sphere(
        ph::Point::from_trans(trans),
        color);
    lhs->draw(selected);
    rhs->draw(selected);
}

void ph::Node<ph::NodeType::Mult>::regist(ph::World& world) {
    world.dynamics_world()->addRigidBody(m_body);
    lhs->regist(world);
    rhs->regist(world);
}
void ph::Node<ph::NodeType::Mult>::remove(ph::World& world) {
    world.dynamics_world()->removeRigidBody(m_body);
    lhs->remove(world);
    rhs->remove(world);
}

void ph::Node<ph::NodeType::Mult>::update() {
    lhs->update();
    rhs->update();
}

void ph::Node<ph::NodeType::Print>::draw(ph::Element const* selected) const {
    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);
    ph::graphics::Color color = { 140, 80, 210 };
    if (this == selected)
        color = color + ph::graphics::Color{ 40, 40, 40 };
    ph::graphics::draw_sphere(
        ph::Point::from_trans(trans),
        color);
    val->draw(selected);
}

void ph::Node<ph::NodeType::Print>::regist(ph::World& world) {
    world.dynamics_world()->addRigidBody(m_body);
    val->regist(world);
}

void ph::Node<ph::NodeType::Print>::remove(ph::World& world) {
    world.dynamics_world()->removeRigidBody(m_body);
    val->remove(world);
}

void ph::Node<ph::NodeType::Print>::update() {
    val->update();
}

void ph::Node<ph::NodeType::If>::draw(ph::Element const* selected) const {
    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);
    ph::graphics::Color color = { 210, 80, 80 };
    if (this == selected)
        color = color + ph::graphics::Color{ 40, 40, 40 };
    ph::graphics::draw_sphere(
        ph::Point::from_trans(trans),
        color);
    cond->draw(selected);
    true_->draw(selected);
    false_->draw(selected);
}

void ph::Node<ph::NodeType::If>::regist(ph::World& world) {
    world.dynamics_world()->addRigidBody(m_body);
    cond->regist(world);
    true_->regist(world);
    false_->regist(world);
}

void ph::Node<ph::NodeType::If>::remove(ph::World& world) {
    world.dynamics_world()->removeRigidBody(m_body);
    cond->remove(world);
    true_->remove(world);
    false_->remove(world);
}

void ph::Node<ph::NodeType::If>::update() {
    cond->update();
    true_->update();
    false_->update();
}

void ph::Literal::draw(ph::Element const* selected) const {
    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);
    ph::graphics::Color color = { 210, 200, 80 };
    if (this == selected)
        color = color + ph::graphics::Color{ 40, 40, 40 };
    ph::graphics::draw_sphere(
        ph::Point::from_trans(trans),
        color);
}

void ph::Literal::update() {
}

void ph::Literal::regist(ph::World& world) {
    world.dynamics_world()->addRigidBody(m_body);
}

void ph::Literal::remove(ph::World& world) {
    world.dynamics_world()->removeRigidBody(m_body);
}

void ph::Undefined::draw(ph::Element const* selected) const {
    btTransform trans;
    m_body->getMotionState()->getWorldTransform(trans);
    ph::graphics::Color color = { 40, 40, 40 };
    if (this == selected)
        color = color + ph::graphics::Color{ 40, 40, 40 };
    ph::graphics::draw_sphere(
        ph::Point::from_trans(trans),
        color);
}

void ph::Undefined::regist(ph::World& world) {
    world.dynamics_world()->addRigidBody(m_body);
}

void ph::Undefined::remove(ph::World& world) {
    world.dynamics_world()->removeRigidBody(m_body);
}



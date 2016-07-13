/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <picojson.h>
#include <fstream>

#include <btBulletDynamicsCommon.h>
#include "ast.hpp"

static std::shared_ptr<ph::Element> loading(picojson::value const& v, ph::Point const& current_position) {
    auto next_position = ph::Point{
        current_position.x + double(std::rand() % 1000) / 10000.0,
        current_position.y + double(std::rand() % 1000) / 10000.0,
        current_position.z + 0.01
    };
    if (v.is<double>())
        return std::make_shared<ph::Literal>(current_position, static_cast<int>(v.get<double>()));
    if (!v.is<picojson::object>())
        return std::make_shared<ph::Literal>(ph::Point{}, 42);
    auto& obj = v.get<picojson::object>();
    auto op = obj.find("op")->second.get<std::string>();
    if (op == "plus") {
        return std::make_shared<ph::Node<ph::NodeType::Plus>>(
            current_position,
            loading(obj.at("lhs"), next_position),
            loading(obj.at("rhs"), next_position)
            );
    } if (op == "mult") {
        return std::make_shared<ph::Node<ph::NodeType::Mult>>(
            current_position,
            loading(obj.at("lhs"), next_position),
            loading(obj.at("rhs"), next_position)
            );
    } if (op == "print") {
        return std::make_shared<ph::Node<ph::NodeType::Print>>(
            current_position,
            loading(obj.at("val"), next_position)
            );
    } else {
        throw ph::Element::invalid_loading_exception{};
    }
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

std::shared_ptr<ph::Element> ph::Element::load(std::string const& filename) {
	std::ifstream input(filename);
	if (input.fail())
		return nullptr;

    picojson::value v;
    input >> v;
    return loading(v, ph::Point{});
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
    // draw_sphere();
    lhs->draw();
    rhs->draw();
}

void ph::Node<ph::NodeType::Plus>::update() {
    lhs->update();
    rhs->update();
}

void ph::Node<ph::NodeType::Mult>::draw() const {
    // draw_sphere();
    lhs->draw();
    rhs->draw();
}

void ph::Node<ph::NodeType::Mult>::update() {
    lhs->update();
    rhs->update();
}

void ph::Node<ph::NodeType::Print>::draw() const {
    val->draw();
}

void ph::Node<ph::NodeType::Print>::update() {
    val->update();
}

void ph::Literal::draw() const {
}

void ph::Literal::update() {
}




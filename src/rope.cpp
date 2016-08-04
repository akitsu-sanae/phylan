/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>

#include "rope.hpp"
#include "ast.hpp"
#include "world.hpp"
#include "graphics.hpp"

ph::Rope::Rope(ph::Element const& l, ph::Element const& r, btSoftBodyWorldInfo& info) {
    m_body = btSoftBodyHelpers::CreateRope(info, l.position(), r.position(), 16, 0);
    m_body->m_cfg.piterations = 4;
    m_body->m_materials[0]->m_kLST = 0.5;
    m_body->setTotalMass(5.0);
    m_body->getCollisionShape()->setMargin(0.01f);
    m_body->appendAnchor(0, l.body());
    m_body->appendAnchor(m_body->m_nodes.size() - 1, r.body());
}

ph::Rope::Rope(ph::Element const& top, btSoftBodyWorldInfo& info) {
    m_body = btSoftBodyHelpers::CreateRope(info, btVector3(0, 10, 0), top.position(), 16, 1);
    m_body->m_cfg.piterations = 4;
    m_body->m_materials[0]->m_kLST = 0.5;
    m_body->setTotalMass(5.0);
    m_body->getCollisionShape()->setMargin(0.01f);
    m_body->appendAnchor(m_body->m_nodes.size() - 1, top.body());
}

ph::Rope::~Rope() {
    delete m_body;
}

void ph::Rope::regist(ph::World& world) {
    world.dynamics_world()->addSoftBody(m_body);
}

void ph::Rope::remove(ph::World& world) {
    world.dynamics_world()->removeSoftBody(m_body);
}

void ph::Rope::draw() const {
    ph::graphics::draw_rope(*m_body);
}

template<ph::NodeType type>
auto node_cast(ph::Element const* e) {
    return dynamic_cast<ph::Node<type> const*>(e);
}

std::vector<std::shared_ptr<ph::Rope>> ph::Rope::set(ph::Element const* element, btSoftBodyWorldInfo& info) {
    if (auto mult_node = node_cast<ph::NodeType::Mult>(element)) {
        auto left_ropes = ph::Rope::set(mult_node->lhs.get(), info);
        left_ropes.push_back(std::make_shared<ph::Rope>(*element, *mult_node->lhs, info));
        auto right_ropes = ph::Rope::set(mult_node->rhs.get(), info);
        right_ropes.push_back(std::make_shared<ph::Rope>(*element, *mult_node->rhs, info));
        left_ropes.insert(std::end(left_ropes), std::begin(right_ropes), std::end(right_ropes));
        return left_ropes;
    } else if (auto plus_node = node_cast<ph::NodeType::Plus>(element)) {
        auto left_ropes = Rope::set(plus_node->lhs.get(), info);
        left_ropes.push_back(std::make_shared<ph::Rope>(*element, *plus_node->lhs, info));
        auto right_ropes = Rope::set(plus_node->rhs.get(), info);
        right_ropes.push_back(std::make_shared<ph::Rope>(*element, *plus_node->rhs, info));
        left_ropes.insert(std::end(left_ropes), std::begin(right_ropes), std::end(right_ropes));
        return left_ropes;
    } else if (auto print_node = node_cast<ph::NodeType::Print>(element)) {
        auto child_ropes = Rope::set(print_node->val.get(), info);
        child_ropes.push_back(std::make_shared<Rope>(*element, *print_node->val, info));
        return child_ropes;
    } else if (auto literal_node = dynamic_cast<Literal const*>(element)) {
        return{};
    }
    return{};
}

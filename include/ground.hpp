/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#ifndef PHYLAN_GROUND_HPP
#define PHYLAN_GROUND_HPP

class btDynamicsWorld;
class btCollisionShape;
class btRigidBody;

namespace ph {

struct Ground {
    explicit Ground(btDynamicsWorld&);
    ~Ground();
    void draw() const;

    static constexpr double height = 0.0;
private:
    btCollisionShape* m_shape;
    btRigidBody* m_body;
};

}

#endif

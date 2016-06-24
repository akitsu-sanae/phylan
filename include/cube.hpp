/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#ifndef PHYLAN_CUBE_HPP
#define PHYLAN_CUBE_HPP

#include <btBulletDynamicsCommon.h>

namespace ph {

struct Cube {
    explicit Cube(btDiscreteDynamicsWorld&);
    ~Cube();
    void draw() const;

    const btRigidBody* body() const {
        return m_body;
    }

    constexpr static double cube_half_length = 0.2;
private:
    btCollisionShape* m_shape;
    btRigidBody* m_body;
};

}

#endif

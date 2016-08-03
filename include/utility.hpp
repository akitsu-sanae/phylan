#ifndef PHYLAN_UTILITY_HPP
#define PHYLAN_UTILITY_HPP

#include <btBulletDynamicsCommon.h>

namespace ph {

struct Point {
    double x;
    double y;
    double z;
    static Point from_trans(btTransform const& trans) {
        return Point{
            trans.getOrigin().getX(),
            trans.getOrigin().getY(),
            trans.getOrigin().getZ()
        };
    }
    static Point from_vec(btVector3 const& vec) {
        return Point{
           vec.x(),
           vec.y(),
           vec.z()
        };
    }
};

}

#endif

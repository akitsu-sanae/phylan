#ifndef PHYLAN_UTILITY_HPP
#define PHYLAN_UTILITY_HPP

#include <btBulletDynamicsCommon.h>

namespace ph {

template<typename T>
inline constexpr T PI() {
    return static_cast<T>(3.141592);
}

struct Point {
    float x;
    float y;
    float z;
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

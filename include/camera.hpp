/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#ifndef PHYLAN_CAMERA_HPP
#define PHYLAN_CAMERA_HPP

#include <array>

namespace ph {

struct Camera {
    explicit Camera();
    void update();
protected:
    std::array<double, 3> camera_position;
    std::array<double, 3> target_position;
    std::array<double, 3> up_direction;
};

}

#endif

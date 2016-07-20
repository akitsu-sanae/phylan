/*============================================================================
  Copyright (C) 2016 akitsu sanae
  https://github.com/akitsu-sanae/phylan
  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/


#include "world.hpp"
#include "ast.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    std::shared_ptr<ph::World> world;
    if (argc == 2)
        world = std::make_shared<ph::World>(argv[1]);
    else
        world = std::make_shared<ph::World>();

    while (world->update()) {
        ;
    }
}

/*============================================================================
Copyright (C) 2016 akitsu sanae
https://github.com/akitsu-sanae/phylan
Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
============================================================================*/

#ifndef PHYLAN_ROPE_HPP
#define PHYLAN_ROPE_HPP

#include <vector>
#include <memory>

class btSoftBody;
struct btSoftBodyWorldInfo;

namespace ph {

struct World;
struct Element;

struct Rope {
    explicit Rope(Element const&, Element const&, btSoftBodyWorldInfo&);
    explicit Rope(Element const&, btSoftBodyWorldInfo&);
    ~Rope();
    void draw() const;
    void regist(ph::World&);
    void remove(ph::World&);

    static std::vector<std::shared_ptr<ph::Rope>> set(std::shared_ptr<Element> const&, btSoftBodyWorldInfo&);
private:
    btSoftBody* m_body;
};

}

#endif

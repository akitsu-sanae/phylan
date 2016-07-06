#include "ast.hpp"

// TODO: loading json data
std::shared_ptr<ph::Element> ph::Element::load(const char* filename) {
    return std::make_shared<ph::Literal>(42);
}



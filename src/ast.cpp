#include <picojson.h>
#include <sstream>
#include "ast.hpp"

static std::shared_ptr<ph::Element> loading(picojson::value const& v) {

    if (v.is<double>())
        return std::make_shared<ph::Literal>(static_cast<int>(v.get<double>()));
    if (!v.is<picojson::object>())
        return std::make_shared<ph::Literal>(42);
    auto& obj = v.get<picojson::object>();
    auto op = obj.find("op")->second.get<std::string>();
    if (op == "plus") {
        return std::make_shared<ph::Node<ph::NodeType::Plus>>(
                loading(obj.at("lhs")),
                loading(obj.at("rhs"))
                );
    } if (op == "mult") {
        return std::make_shared<ph::Node<ph::NodeType::Mult>>(
                loading(obj.at("lhs")),
                loading(obj.at("rhs"))
                );
    } if (op == "print") {
        return std::make_shared<ph::Node<ph::NodeType::Print>>(
                loading(obj.at("val"))
                );
    } else {
        throw ph::Element::invalid_loading_exception{};
    }
}

std::shared_ptr<ph::Element> ph::Element::load(std::string const& filename) {
    std::stringstream ss;
    ss << R"(
{
    "op" : "plus",
    "lhs" : 12,
    "rhs" : {
        "op" : "mult",
        "lhs" : 43,
        "rhs" : 12
    }
}
)";

    picojson::value v;
    ss >> v;
    return loading(v);
}



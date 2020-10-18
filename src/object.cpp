#include "object.hpp"

Object::Object(P position, P direction, float weight)
    : v_(position, direction), weight_(weight) {}

#include "object.hpp"
#include "helpers/misc.hpp"

Object::Object(P position, P direction, int weight)
    : v_(position, direction), weight_(weight) {}

O::O(P position, P direction, int diameter)
    : Object(position, direction,
             constants::pi * diameter * diameter * diameter / 6),
      diameter_(diameter) {}

O::~O() {}

void O::RenderParameters(std::vector<float> &p) {}

N::N(P position, int head, int body, int hands, int legs, int guns)
    : Object(position, position, 0),
      head_(head),
      body_(body),
      hands_(hands),
      legs_(legs),
      guns_(guns){};

N::~N(){};

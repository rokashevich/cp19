#include "object.hpp"
#include "helpers/misc.hpp"

Object::Object(float x, float y, float z, int weight)
    : x_(x), y_(y), z_(z), weight_(weight) {}

O::O(float x, float y, float z, int diameter)
    : Object(x, y, z, constants::pi * diameter * diameter * diameter / 6),
      diameter_(diameter) {}

O::~O() {}

void O::RenderParameters(std::vector<float> &p) {}

N::N(float x, float y, float z, int head, int body, int hands, int legs,
     int guns)
    : Object(x, y, z, 0),
      head_(head),
      body_(body),
      hands_(hands),
      legs_(legs),
      guns_(guns){};

N::~N(){};

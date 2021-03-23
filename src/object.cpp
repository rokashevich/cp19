#include "object.hpp"

Object::Object(Vec v, float weight) : v_(v), weight_(weight) {
  offsets_.resize(1);
  offsets_.at(0) = {0, 0, 0, 0};
}

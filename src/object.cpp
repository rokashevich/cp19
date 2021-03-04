#include "object.hpp"

Object::Object(Vec v, float weight) : v_(v), weight_(weight) {
  coords_params_.resize(1);
  coords_params_.at(0) = {0, 0, 0, 0};
}

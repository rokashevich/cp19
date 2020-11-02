#include "object.hpp"

Object::Object(float weight) : weight_(weight) {
  coords_params_.resize(1);
  coords_params_.at(0) = {0, 0, 0, 0};
}

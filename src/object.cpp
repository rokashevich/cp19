#include "object.hpp"

#include <glm/vec3.hpp>
Object::Object(Vec v, float weight, glm::vec3 angles, glm::vec3 params)
    : v_(v), weight_(weight) {
  offsets_.resize(1);
  offsets_.at(0) = {0, 0, 0};
}

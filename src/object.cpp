#include "object.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "tribool.hpp"

Object::Object(glm::vec3 coords, glm::vec3 angles, glm::vec4 params) {
  coords_.push_back(coords.x);
  coords_.push_back(coords.y);
  coords_.push_back(coords.z);
  angles_.push_back(angles.x);
  angles_.push_back(angles.y);
  angles_.push_back(angles.z);
  params_.push_back(params.x);
  params_.push_back(params.y);
  params_.push_back(params.z);
  params_.push_back(params.w);
}

void Object::Move(tribool backward_forward, tribool left_right,
                  tribool down_up) {
  std::cout << backward_forward << left_right << down_up << std::endl;
}

// TODO OLD
// Object::Object(Vec v, float weight, glm::vec3 angles, glm::vec3 params)
//     : v_(v), weight_(weight) {
//   offsets_old_.resize(1);
//   offsets_old_.at(0) = {0, 0, 0};
// }
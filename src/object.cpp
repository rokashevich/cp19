#include "object.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "t.hpp"

Object::Object(glm::vec3 c, glm::vec3 angles, glm::vec4 params) {
  static unsigned int max_id{0};
  id_ = ++max_id;

  coords.push_back(c.x);
  coords.push_back(c.y);
  coords.push_back(c.z);
  angles_.push_back(angles.x);
  angles_.push_back(angles.y);
  angles_.push_back(angles.z);
  params_.push_back(params.x);
  params_.push_back(params.y);
  params_.push_back(params.z);
  params_.push_back(params.w);
}

// TODO OLD
// Object::Object(V v, float weight, glm::vec3 angles, glm::vec3 params)
//     : v_(v), weight_(weight) {
//   offsets_old_.resize(1);
//   offsets_old_.at(0) = {0, 0, 0};
// }
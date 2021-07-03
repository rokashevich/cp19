#include "object.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
Object::Object(glm::vec3 coords, glm::vec3 angles, glm::vec4 params)
    : coords_{coords}, angles_{angles}, params_{params} {}
// TODO OLD
Object::Object(Vec v, float weight, glm::vec3 angles, glm::vec3 params)
    : v_(v), weight_(weight) {
  offsets_old_.resize(1);
  offsets_old_.at(0) = {0, 0, 0};
}

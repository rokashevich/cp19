#pragma once
#include <glm/vec3.hpp>
#include <vector>

#include "generator_shape.hpp"
#include "object.hpp"

class Wall : public Object {
  static const std::vector<float> vertices_buffer_;

 public:
  Wall(glm::vec3 coords, glm::vec3 angles);
  ~Wall();
  void Step() final {}
};

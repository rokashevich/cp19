#pragma once
#include <glm/vec3.hpp>
#include <vector>

#include "generator_shape.hpp"
#include "object.hpp"

// Стена.
class Wall : public Object, Shape<Wall> {
  static const std::vector<float> vertices_buffer_;

 public:
  Wall(glm::vec3 coords, glm::vec3 angles);
  // TODO OLD
  // Wall(V v = V(), glm::vec3 angles = {0, 0, 0},
  //      glm::vec3 params = {0, 0, 0});
  ~Wall();
  void Step() final {}
  const std::vector<float>* ShapeVerticesBuffer() final;
};

#pragma once
#include <vector>

#include "generator_shape.hpp"
#include "object.hpp"

// Стена.
class ObjectWall : public Object {
  static const std::vector<float> vertices_buffer_;

  int health_;

 public:
  ObjectWall(P position, int orientation_helath);
  ~ObjectWall();
  float w() final { return health_; }
  const float* ShapeVerticesBuffer() final;
  std::size_t SizeofShapeVerticesBuffer() final;
};

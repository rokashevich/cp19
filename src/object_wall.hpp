#pragma once
#include <vector>

#include "generator_shape.hpp"
#include "object.hpp"

// Стена.
class ObjectWall : public Object {
  static const std::vector<float> vertices_buffer_;
  static const int num_vertices_;

  int health_;

 public:
  ObjectWall(int helath);
  ~ObjectWall();
  float w() final { return health_; }
  const float* VerticesBuffer() final;
  std::size_t SizeofVerticesBuffer() final;
  int NumVertices() final;
};

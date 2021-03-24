#pragma once
#include <vector>

#include "generator_shape.hpp"
#include "object.hpp"

// Стена.
class Wall : public Object, Shape<Wall> {
  static const std::vector<float> vertices_buffer_;
  int health_;

 public:
  Wall(Vec v = Vec(), int orientation_helath = 100);
  ~Wall();
  void Step() final { Object::offsets_.at(0).at(3) = health_; }
  const std::vector<float>* ShapeVerticesBuffer() final;
};

#pragma once
#include <vector>

#include "generator_shape.hpp"
#include "object.hpp"

// Стена.
class ObjectWall : public Object, Shape<ObjectWall> {
  static const std::vector<float> vertices_buffer_;
  int health_;

 public:
  ObjectWall(Vec v = Vec(), int orientation_helath = 100);
  ~ObjectWall();
  void Step() final { Object::offsets_.at(0).at(3) = health_; }
  const std::vector<float>* ShapeVerticesBuffer() final;
};

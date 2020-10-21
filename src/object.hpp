#pragma once

#include <vector>

#include "point.hpp"
#include "shape.hpp"
#include "vec.hpp"

// Базовый класс физического объекта игрового мира.
class Object {
  Vec v_;
  float weight_;

 public:
  Object(P position, P direction, float weight);
  virtual ~Object() {}

  float& x() { return v_.Begin().x; }
  float& y() { return v_.Begin().y; }
  float& z() { return v_.Begin().z; }
  virtual void RenderParameters(std::vector<float>& p) = 0;  // Удалить!
};

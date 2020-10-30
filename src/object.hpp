#pragma once

#include <cstddef>
#include <vector>

#include "point.hpp"
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
  virtual float w() = 0;
  virtual const float* VerticesBuffer() = 0;
  virtual std::size_t SizeofVerticesBuffer() = 0;
  virtual int NumVertices() = 0;
};

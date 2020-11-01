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

  // Возвращает координты вершин базовой формы: параллелепипеда, куба, шара, и
  // т.д. в виде: x1,y1,z1,x2,y2,z2,..., где каждая тройка координат
  // представляет собой тругольник. Эта информация нужна будет только рендеру.
  virtual const std::vector<float>* ShapeVerticesBuffer() = 0;

  // del
  float& x() { return v_.Begin().x; }
  float& y() { return v_.Begin().y; }
  float& z() { return v_.Begin().z; }
  virtual float w() = 0;
};

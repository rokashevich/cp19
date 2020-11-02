#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "point.hpp"
#include "vec.hpp"

// Базовый класс физического объекта игрового мира.
class Object {
  float weight_;

 protected:
  std::vector<std::array<float, 4>> coords_params_;

 public:
  Object(float weight);
  virtual ~Object() {}

  // Возвращает координты вершин базовой формы: параллелепипеда, куба, шара, и
  // т.д. в виде: x1,y1,z1,x2,y2,z2,..., где каждая тройка координат
  // представляет собой тругольник. Передаётся в рендер.
  virtual const std::vector<float>* ShapeVerticesBuffer() = 0;

  // Для обновления параметров для шейдера.
  virtual void Step() {}

  // Массив координат базовых объектов для инстансированного рендеринга.
  virtual const std::vector<std::array<float, 4>>* CoordsParams() {
    return &coords_params_;
  }
};

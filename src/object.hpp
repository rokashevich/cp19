#pragma once

#include <array>
#include <cstddef>
#include <iostream>
#include <vector>

#include "point.hpp"
#include "vec.hpp"

// Базовый класс физического объекта игрового мира.
class Object {
  Vec v_;
  float weight_;
  Object* owner_;
  Point attachment_point_;

 protected:
  std::vector<std::array<float, 4>> coords_params_;

 public:
  Object(Vec v = Vec(), float weight = 0);
  virtual ~Object() {}

  Vec* V() { return &v_; }

  // Возвращает координты вершин базовой формы: параллелепипеда, куба, шара, и
  // т.д. в виде: x1,y1,z1,x2,y2,z2,..., где каждая тройка координат
  // представляет собой тругольник. Передаётся в рендер.
  virtual const std::vector<float>* ShapeVerticesBuffer() = 0;

  // Для обновления параметров для шейдера.
  virtual void Step() {}

  // Массив координат базовых объектов для инстансированного рендеринга.
  virtual const std::vector<std::array<float, 4>>& CoordsParams() {
    return coords_params_;
  }

  void Owner(Object* owner = nullptr) { owner_ = owner; }
  Object* Owner() { return owner_; }
  Point* AttachmentPoint() { return &attachment_point_; }
};

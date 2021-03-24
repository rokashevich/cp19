#pragma once

#include <array>
#include <cstddef>
#include <iostream>
#include <vector>

#include "point.hpp"
#include "vec.hpp"

struct ShapeInfo {
  std::vector<float> vertices_buffer;
  const char* vertex_shader;
  const char* pixel_shader;
};

template <class T>
class Shape {
  static const ShapeInfo objects_static_info_;
  static const std::vector<float> vertices_buffer_;

 public:
  static auto& ShapeVerticesBuffer2() { return Shape::vertices_buffer_; }
  static auto& StaticInfo() { return objects_static_info_; }
};

// Базовый класс физического объекта игрового мира.
class Object {
  P orientation_;  // В какую сторону смотрит объект.

  P motion_external_;  // Запрошенное движение извне (кнопками, Ai).
  P motion_internal_;  // Фактическое движение (блокируемое анимацией).
  int timer_inertia_;  // Таймер отработки анимации.
  Vec v_;
  int weight_;  // >0 обычный объект, =0 стена, <0 артефактphy

 protected:
  // Координаты смещений базовых объектов.
  std::vector<std::array<float, 4>> offsets_;

 public:
  Object(Vec v = Vec(), float weight = 0);
  virtual ~Object() {}

  P& GetOrientation() { return orientation_; }
  void SetOrientation(P p) { orientation_ = p; }

  void SetMotion(P& p) { motion_external_ = p; }
  Vec& V() { return v_; }

  // Возвращает координты вершин базовой формы: параллелепипеда, куба, шара, и
  // т.д. в виде: x1,y1,z1,x2,y2,z2,..., где каждая тройка координат
  // представляет собой тругольник. Передаётся в рендер.
  virtual const std::vector<float>* ShapeVerticesBuffer() = 0;

  // Для обновления параметров для шейдера.
  virtual void Step() {}

  // Массив координат базовых объектов для инстансированного рендеринга.
  virtual const std::vector<std::array<float, 4>>& Offsets() {
    return offsets_;
  }

  int Weight() { return weight_; }
};

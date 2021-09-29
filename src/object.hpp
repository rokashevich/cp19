#pragma once

#include <array>
#include <cstddef>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <vector>

#include "t.hpp"

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
 protected:
  static constexpr int num_instances_ = 1;
  std::vector<float> coords_;
  std::vector<float> angles_;
  std::vector<float> params_;

 public:
  static constexpr int num_coords = 3;
  static constexpr int num_angles = 3;
  static constexpr int num_params = 4;
  Object(glm::vec3 coords = {0, 0, 0}, glm::vec3 angles = {0, 0, 0},
         glm::vec4 params = {0, 0, 0, 0});
  virtual ~Object() {}
  const auto& Coords() { return coords_; }
  const auto& Angles() { return angles_; }
  const auto& Params() { return params_; }
  // Из скольки однотипных фигур составлен объект. Например в стене или в
  // снараяде по одной. Человек же состоит из 8: голова, туловище, два плеча,
  // два предплечья, два бедра, две голени.
  virtual int NumInstances() { return Object::num_instances_; }
  // Для обновления параметров для шейдера.
  virtual void Step() {}

  void Move(tribool backward_forward, tribool left_right, tribool down_up);

 public:
  // TODO OLD
  // P orientation_;  // В какую сторону смотрит объект.
  // P motion_external_;  // Запрошенное движение извне (кнопками, Ai).
  // P motion_internal_;  // Фактическое движение (блокируемое анимацией).
  int timer_inertia_;  // Таймер отработки анимации.
  V v_;
  int weight_;  // >0 обычный объект, =0 стена, <0 артефактphy
  // std::vector<std::array<float, 3>> offsets_old_;
  // std::vector<std::array<float, 3>> angles_old_;
  // std::vector<std::array<float, 3>> params_old_;
  // // Object(V v = V(), float weight = 0, glm::vec3 angles = {0, 0, 0},
  //        glm::vec3 params = {0, 0, 0});

  // P& GetOrientation() { return orientation_; }
  // void SetOrientation(P p) { orientation_ = p; }

  // void SetMotion(P& p) { motion_external_ = p; }
  V& v() { return v_; }

  // Возвращает координты вершин базовой формы: параллелепипеда, куба, шара, и
  // т.д. в виде: x1,y1,z1,x2,y2,z2,..., где каждая тройка координат
  // представляет собой тругольник. Передаётся в рендер.
  virtual const std::vector<float>* ShapeVerticesBuffer() = 0;

  // virtual const std::vector<std::array<float, 3>>& Offsets() {
  //   return offsets_old_;
  // }

  int Weight() { return weight_; }
};

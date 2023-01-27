#pragma once

#include <array>
#include <cstddef>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <tuple>
#include <vector>

#include "t.hpp"

// Базовый класс физического объекта игрового мира.
class Object {
 protected:
  static constexpr int num_instances_ = 1;

 public:
  glm::vec3 coords;
  glm::vec3 angles;
  glm::vec4 params;
  tribool backward_forward;
  tribool left_right;
  tribool up_down;
  static constexpr int num_coords = 3;
  static constexpr int num_angles = 3;
  static constexpr int num_params = 4;
  Object(glm::vec3 coords = {4, 4, 4}, glm::vec3 angles = {0, 0, 0},
         glm::vec4 params = {0, 0, 0, 0})
      : coords{coords}, angles{angles}, params{params} {};
  virtual ~Object() {}

  // Из скольки однотипных фигур составлен объект. Например в стене или в
  // снараяде по одной. Человек же состоит из 8: голова, туловище, два
  // плеча, два предплечья, два бедра, две голени.
  virtual int NumInstances() { return Object::num_instances_; }
  // Для обновления параметров для шейдера.
  virtual void Step() {}

  // Objects speed: 0 = object cant move, 100 = one cell/second
  virtual int SpeedCoefficient() { return 1; }

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

  // virtual const std::vector<std::array<float, 3>>& Offsets() {
  //   return offsets_old_;
  // }

  int Weight() { return weight_; }
};

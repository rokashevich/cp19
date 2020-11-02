#pragma once
#include <cstddef>
#include <vector>

#include "generator_shape.hpp"
#include "object.hpp"

// Снаряд.
// Все снаряды одинаковые, отличаются скоростью, размером и плотностью
// - из этих переменных расчитывается убойная сила конкретного снаряда.
class ObjectMissile : public Object {
  static const std::vector<float> vertices_buffer_;
  const float diameter_;

 public:
  ObjectMissile(float diameter = 1);
  ~ObjectMissile();
  void Step() final { Object::coords_params_.at(0).at(3) = diameter_ / 2; }
  const std::vector<float>* ShapeVerticesBuffer() final;
};

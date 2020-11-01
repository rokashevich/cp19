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
  ObjectMissile(P position, P direction, float diameter);
  ~ObjectMissile();
  float w() final { return diameter_ / 2; }
  const std::vector<float>* ShapeVerticesBuffer() final;
};

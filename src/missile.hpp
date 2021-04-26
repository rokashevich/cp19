#pragma once
#include <cstddef>
#include <vector>

#include "generator_shape.hpp"
#include "object.hpp"

// Снаряд.
// Все снаряды одинаковые, отличаются скоростью, размером и плотностью
// - из этих переменных расчитывается убойная сила конкретного снаряда.
class Missile : public Object, Shape<Missile> {
  static const std::vector<float> vertices_buffer_;
  const float diameter_;

 public:
  Missile(Vec v = Vec(), float diameter = 1);
  ~Missile();
  void Step() final {
    // Object::offsets_.at(0).at(3) = diameter_ / 2;
  }
  const std::vector<float>* ShapeVerticesBuffer() final;
};

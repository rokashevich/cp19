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
  // Т.к. шар генерируется единичного радиуса, т.е. диаметром 2, то коэффициент
  // масштабирования делим на два, чтобы как будто диаметр шара = 1.
  float w() final { return diameter_ / 2; }
  const std::vector<float>* ShapeVerticesBuffer() final;
};

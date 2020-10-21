#pragma once
#include <vector>

#include "object.hpp"
#include "shape.hpp"
#include "shapegenerator.hpp"

// Снаряд.
// Все снаряды одинаковые, отличаются скоростью, размером и плотностью
// - из этих переменных расчитывается убойная сила конкретного снаряда.
class ObjectMissile : public Object, Shape<ObjectMissile> {
  const float diameter_;
  const float density_;

 public:
  ObjectMissile(P position, P direction, float diameter, float density);
  ~ObjectMissile();
  void RenderParameters(std::vector<float>& p) override;
};

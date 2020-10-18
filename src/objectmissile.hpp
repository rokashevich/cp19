#pragma once
#include "object.hpp"
// Снаряд.
// Все снаряды одинаковые, отличаются скоростью, размером и плотностью
// - из этих переменных расчитывается убойная сила конкретного снаряда.
class ObjectMissile : public Object {
  static const std::vector<float> shape_vertices_;
  float diameter_;
  float density_;

 public:
  static int ShapeVerticesCount();
  static int ShapeBytesCount();
  static const float* ShapeData();

  ObjectMissile(P position, P direction, float diameter, float density);
  ~ObjectMissile();
  void RenderParameters(std::vector<float>& p) override;
};

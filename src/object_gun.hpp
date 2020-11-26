#pragma once
#include <cstddef>
#include <vector>

#include "generator_shape.hpp"
#include "object.hpp"

// Снаряд.
// Все снаряды одинаковые, отличаются скоростью, размером и плотностью
// - из этих переменных расчитывается убойная сила конкретного снаряда.
class ObjectGun : public Object, Shape<ObjectGun> {
  static const std::vector<float> vertices_buffer_;
  const int num_barrels_;
  const float length_;
  const float caliber_;

 public:
  ObjectGun(Vec v = Vec(), int num_barrels = 1, float length = 1,
            float caliber = 0.1)
      : Object(v, num_barrels * length * caliber),
        num_barrels_(num_barrels),
        length_(length),
        caliber_(caliber) {}

  ~ObjectGun();
  void Step() final { Object::coords_params_.at(0).at(3) = 1; }
  const std::vector<float>* ShapeVerticesBuffer() final;
};

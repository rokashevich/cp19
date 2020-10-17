#pragma once

#include <vector>

#include "physics.hpp"
#include "point.hpp"
#include "vec.hpp"

class Physics;

// Базовый класс физического объекта игрового мира.
class Object {
  Vec v_;
  float weight_;

 public:
  friend class Physics;
  Object(P position, P direction, float weight);
  virtual ~Object() {}
  virtual void RenderParameters(std::vector<float> &p) = 0;
  float& x() { return v_.Begin().x; }
  float& y() { return v_.Begin().y; }
  float& z() { return v_.Begin().z; }
};

// Снаряд.
// Все снаряды одинаковые, отличаются скоростью, размером и плотностью
// - из этих переменных расчитывается убойная сила конкретного снаряда.
class O : public Object {
  static const std::vector<float> shape_vertices_;
  float diameter_;
  float density_;

 public:
  static int ShapeVerticesCount();
  static int ShapeBytesCount();
  static const float* ShapeData();

  O(P position, P direction, float diameter, float density);
  ~O();
  void RenderParameters(std::vector<float>& p) override;
};

// Солдат.
class N : public Object {
  static const std::vector<float> shape_vertices_;
  int legs_;
  int body_;
  int head_;
  int arms_;
  int gun_;

 public:
  static int ShapeVerticesCount();
  static int ShapeBytesCount();
  static const float* ShapeData();

  N(P position, P direction, int legs = 9, int body = 9, int head = 9,
    int arms = 9, int gun = 9);
  ~N();
  virtual void RenderParameters(std::vector<float>& p) override;
};

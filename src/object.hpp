#pragma once

#include <vector>

#include "physics.hpp"
#include "point.hpp"
#include "vec.hpp"

// Базовый класс физического объекта игрового мира.
class Physics;
class Object
{
 public:
  friend class Physics;
  Object(P position, P direction, int weight);
  virtual ~Object() {}
  virtual void RenderParameters(std::vector<float> &p) = 0;
  float& x() { return v_.Begin().x; }
  float& y() { return v_.Begin().y; }
  float& z() { return v_.Begin().z; }

 protected:
  Vec v_;
  int weight_;
};

// Снаряд. Все снаряды одинаковые, отличаются размером, скоростью и,
// соответственно, убойной силой.
class O : public Object {
 public:
  static int ShapeVerticesCount();
  static int ShapeBytesCount();
  static const float* ShapeData();

  O(P position, P direction, int diameter);
  ~O();
  void RenderParameters(std::vector<float>& p) override;

 private:
  static const std::vector<float> shape_vertices_;
  int diameter_;
};

class N : public Object {
 public:
  static int ShapeVerticesCount();
  static int ShapeBytesCount();
  static const float* ShapeData();

  N(P position, P direction, int head, int body = 0, int hands = 0,
    int legs = 0, int guns = 0);
  ~N();
  virtual void RenderParameters(std::vector<float>& p) override;

 private:
  static const std::vector<float> shape_vertices_;
  int head_;
  int body_;
  int hands_;
  int legs_;
  int guns_;
};

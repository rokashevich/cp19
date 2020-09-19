#pragma once

#include <vector>

#include "physics.hpp"
#include "vec.hpp"

// Базовый класс физического объекта игрового мира.
class Physics;
class Object
{
 public:
  friend class Physics;
  Object(float x, float y, float z, int weight);
  virtual ~Object() {}
  virtual void RenderParameters(std::vector<float> &p) = 0;

 protected:
  float x_;
  float y_;
  float z_;
  int weight_;
};

// Снаряд. Все снаряды одинаковые, отличаются размером, скоростью и,
// соответственно, убойной силой.
class O : public Object {
 public:
  O(float x, float y, float z, int diameter);
  ~O();
  virtual void RenderParameters(std::vector<float> &p);
  inline constexpr static float vertices[] = {
      -0.05, -0.05, -0.05, -0.05, -0.05,
      0.05,  0.05,  -0.05, -0.05,  // Нижняя крышка.
      0.05,  -0.05, 0.05,  -0.05, -0.05,
      0.05,  0.05,  -0.05, -0.05,

      -0.05, 0.05,  -0.05, -0.05, 0.05,
      0.05,  0.05,  0.05,  -0.05,  // Верхняя крышка.
      0.05,  0.05,  0.05,  -0.05, 0.05,
      0.05,  0.05,  0.05,  -0.05,

      -0.05, -0.05, 0.05,  -0.05, 0.05,
      0.05,  0.05,  -0.05, 0.05,  // Стенка xy.
      0.05,  0.05,  0.05,  -0.05, 0.05,
      0.05,  0.05,  -0.05, 0.05,

      0.05,  -0.05, -0.05, 0.05,  0.05,
      -0.05, 0.05,  -0.05, 0.05,  // Стенка zy.
      0.05,  0.05,  0.05,  0.05,  0.05,
      -0.05, 0.05,  -0.05, 0.05,

      -0.05, -0.05, -0.05, -0.05, 0.05,
      -0.05, 0.05,  -0.05, -0.05,  // Стенка -xy.
      0.05,  0.05,  -0.05, -0.05, 0.05,
      -0.05, 0.05,  -0.05, -0.05,

      -0.05, -0.05, -0.05, -0.05, 0.05,
      -0.05, -0.05, -0.05, 0.05,  // Стенка -zy.
      -0.05, 0.05,  0.05,  -0.05, 0.05,
      -0.05, -0.05, -0.05, 0.05,
  };

 private:
  int diameter_;
};

class N : public Object {
 public:
  N(float x, float y, float z, int head, int body, int hands, int legs,
    int guns);
  ~N();
  inline constexpr static float vertices[] = {
      -0.1, -0.1, -0.1, -0.1, -0.1, 0.1,  0.1,  -0.1, -0.1,  // Нижняя крышка.
      0.1,  -0.1, 0.1,  -0.1, -0.1, 0.1,  0.1,  -0.1, -0.1,

      -0.1, 0.1,  -0.1, -0.1, 0.1,  0.1,  0.1,  0.1,  -0.1,  // Верхняя крышка.
      0.1,  0.1,  0.1,  -0.1, 0.1,  0.1,  0.1,  0.1,  -0.1,

      -0.1, -0.1, 0.1,  -0.1, 0.1,  0.1,  0.1,  -0.1, 0.1,  // Стенка xy.
      0.1,  0.1,  0.1,  -0.1, 0.1,  0.1,  0.1,  -0.1, 0.1,

      0.1,  -0.1, -0.1, 0.1,  0.1,  -0.1, 0.1,  -0.1, 0.1,  // Стенка zy.
      0.1,  0.1,  0.1,  0.1,  0.1,  -0.1, 0.1,  -0.1, 0.1,

      -0.1, -0.1, -0.1, -0.1, 0.1,  -0.1, 0.1,  -0.1, -0.1,  // Стенка -xy.
      0.1,  0.1,  -0.1, -0.1, 0.1,  -0.1, 0.1,  -0.1, -0.1,

      -0.1, -0.1, -0.1, -0.1, 0.1,  -0.1, -0.1, -0.1, 0.1,  // Стенка -zy.
      -0.1, 0.1,  0.1,  -0.1, 0.1,  -0.1, -0.1, -0.1, 0.1,
  };

 private:
  int head_;
  int body_;
  int hands_;
  int legs_;
  int guns_;
};

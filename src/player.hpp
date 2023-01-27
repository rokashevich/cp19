#pragma once
#include <iostream>
#include <vector>

#include "animation.hpp"
#include "object.hpp"
// #include "point.hpp"
// #include "vec.hpp"
// Солдат.
class Player : public Object {
  // Человечек состоит из 8 фигур: голова, туловище, два плеча, два предплечья,
  // два бедра и две голени.
  // static constexpr int num_instances_ = 1;  // 8;
  static const std::vector<float> vertices_buffer_;

 public:
  Player(glm::vec3 coords, glm::vec3 angles, int head, int body, int arms,
         int legs);
  ~Player();
  int NumInstances() final;
  void Step() final;
};

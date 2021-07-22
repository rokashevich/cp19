#pragma once
#include <iostream>
#include <vector>

#include "object.hpp"
#include "point.hpp"
#include "vec.hpp"
// Солдат.
class Player : public Object, Shape<Player> {
  static constexpr int num_shapes_ = 4;
  static const std::vector<float> vertices_buffer_;
  int legs_;
  int body_;
  int head_;
  int arms_;
  int gun_;

 public:
  Player(glm::vec3 coords, glm::vec3 angles);
  ~Player();
  int NumInstances() final;
  void Step() final;
  // TODO OLD
  //  Player(Vec v = Vec(), int legs(9), int body = 9, int head = 9, int arms =
  // 9);

  const std::vector<float>* ShapeVerticesBuffer() final;
};

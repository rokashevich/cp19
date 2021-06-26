#pragma once
#include <iostream>
#include <vector>

#include "object.hpp"
#include "point.hpp"
#include "vec.hpp"
// Солдат.
class Player : public Object, Shape<Player> {
  static const std::vector<float> vertices_buffer_;
  int legs_;
  int body_;
  int head_;
  int arms_;
  int gun_;

 public:
  Player(Vec v = Vec(), int legs = 9, int body = 9, int head = 9, int arms = 9);
  ~Player();
  void Step() final {
    Object::offsets_.at(0).at(0) = 0;
    Object::offsets_.at(0).at(1) = 0;
    // Object::offsets_.at(0).at(3) = 1;

    Object::offsets_.at(1).at(1) = -1.25;
    Object::offsets_.at(1).at(2) = 0.25;
    // Object::offsets_.at(1).at(3) = 1.5;
  }
  const std::vector<float>* ShapeVerticesBuffer() final;
};
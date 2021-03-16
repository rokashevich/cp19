#pragma once
#include <iostream>
#include <vector>

#include "object.hpp"
#include "point.hpp"
#include "vec.hpp"
// Солдат.
class ObjectPlayer : public Object, Shape<ObjectPlayer> {
  static const std::vector<float> vertices_buffer_;
  int legs_;
  int body_;
  int head_;
  int arms_;
  int gun_;

 public:
  ObjectPlayer(Vec v = Vec(), int legs = 9, int body = 9, int head = 9,
               int arms = 9);
  ~ObjectPlayer();
  void Step() final {
    Object::coords_params_.at(0).at(0) = 0;
    Object::coords_params_.at(0).at(1) = 0;
    Object::coords_params_.at(0).at(3) = 1;

    Object::coords_params_.at(1).at(1) = -1.25;
    Object::coords_params_.at(1).at(2) = 0.25;
    Object::coords_params_.at(1).at(3) = 1.5;
  }
  const std::vector<float>* ShapeVerticesBuffer() final;
};

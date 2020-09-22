#pragma once

#include "point.hpp"

class Vec {
  P position_;
  P direction_;

 public:
  // Конструктор покоящегося тела.
  Vec(P position, P direction) : position_(position), direction_(direction) {}
  P& Position() { return position_; }
};

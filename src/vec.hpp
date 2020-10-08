#pragma once

#include "point.hpp"

class Vec {
  P begin_;
  P end_;

 public:
  // Конструктор покоящегося тела.
  Vec(P begin, P end) : begin_(begin), end_(end) {}
  P& Begin() { return begin_; }
  P& End() { return end_; }
};

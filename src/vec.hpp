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

  friend Vec operator+(Vec lhs, const Vec& rhs) {
    // Под действием rhs изменится только lhs.end_.
    lhs.end_ = rhs.end_ + (lhs.end_ - rhs.begin_);
    return lhs;
  }
};

#pragma once

#include "point.hpp"

class Vec {
  P begin_;
  P end_;

 public:
  Vec() : begin_(0, 0, 0), end_(begin_) {}
  Vec(float x, float y, float z) : begin_(x, y, z), end_(begin_) {}
  Vec(float x, float y, float z, float p, float q, float m)
      : begin_(x, y, z), end_(p, q, m) {}
  Vec(P begin, P end) : begin_(begin), end_(end) {}
  P Begin() { return begin_; }
  P End() { return end_; }

  friend Vec operator+(Vec lhs, const Vec& rhs) {
    // Под действием rhs изменится только lhs.end_.
    lhs.end_ = rhs.end_ + (lhs.end_ - rhs.begin_);
    return lhs;
  }
};

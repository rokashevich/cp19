#pragma once

#include <cmath>
#include <iostream>

#include "point.hpp"

class Vec {
  P begin_;
  P end_;

 public:
  Vec() : begin_(0, 0, 0), end_(begin_) {}
  Vec(float x, float y, float z) : begin_(0, 0, 0), end_(x, y, z) {}
  Vec(float x, float y, float z, float p, float q, float m)
      : begin_(x, y, z), end_(p, q, m) {}
  Vec(P begin, P end) : begin_(begin), end_(end) {}
  P& Begin() { return begin_; }
  P& End() { return end_; }

  friend bool operator==(const Vec& lhs, const Vec& rhs) {
    return lhs.begin_ == rhs.begin_ && lhs.end_ == rhs.end_;
  }

  friend Vec operator+(Vec lhs, const Vec& rhs) {
    // Под действием rhs изменится только lhs.end_.
    lhs.end_ = lhs.end_ + (lhs.end_ - rhs.begin_);
    return lhs;
  }

  friend Vec operator/(Vec lhs, const int factor) {
    lhs.end_.x = lhs.begin_.x + std::fabs(lhs.end_.x - lhs.begin_.x) / factor;
    lhs.end_.y = lhs.begin_.y + std::fabs(lhs.end_.y - lhs.begin_.y) / factor;
    return lhs;
  }

  friend Vec operator>>(Vec lhs, const float fraction) {
    const float dx = (lhs.End().x - lhs.Begin().x) * fraction;
    const float dy = (lhs.End().y - lhs.Begin().y) * fraction;
    const float dz = (lhs.End().z - lhs.Begin().z) * fraction;
    //    std::cout << "!" << lhs.End().x << std::endl;

    lhs.Begin().x += dx;
    lhs.Begin().y += dy;
    lhs.Begin().z += dz;
    lhs.End().x += dx;
    lhs.End().y += dy;
    lhs.End().z += dz;
    return lhs;
  }

 private:
};

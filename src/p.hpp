#pragma once
#include <cmath>
#include <iostream>

class P {
  float x_;
  float y_;
  float z_;

 public:
  P() : x_{0}, y_{0}, z_{0} {}
  P(float x, float y, float z) : x_{x}, y_{y}, z_{z} {}

  float x() { return x_; }
  float y() { return y_; }
  float z() { return z_; }

  void x(float x) { x_ = x; }
  void y(float y) { y_ = y; }
  void z(float z) { z_ = z; }

  float DistanceToSquared(P point) {
    return pow(x_ - point.x_, 2) + pow(y_ - point.y_, 2) +
           pow(z_ - point.z_, 2);
  }

  float DistanceTo(P point) { return sqrt(DistanceToSquared(point)); }

  P& operator+=(const P& rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    return *this;
  }

  friend bool operator==(const P& lhs, const P& rhs) {
    return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_ && lhs.z_ == rhs.z_;
  }

  friend P operator+(P lhs, const P& rhs) {
    return P{lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_};
  }

  friend P operator-(P lhs, const P& rhs) {
    return P{lhs.x_ - rhs.x_, lhs.y_ - rhs.y_, lhs.z_ - rhs.z_};
  }

  friend std::ostream& operator<<(std::ostream& os, const P& rhs) {
    os << rhs.x_ << " " << rhs.y_ << " " << rhs.z_;
    return os;
  }
};

#pragma once

// Всякие классы-типы, которые пока не устоканались в логике игры и постоянно
// меняются, поэтому пока в этом общем файле. По мере устаканивания - вынести
// в отдельный файл.

#include <cmath>
#include <iostream>
#include <ostream>

// Трёхмерная точка.
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

// Вектор-отрезок из одной точки в другую.
class V {
  P begin_;
  P end_;

 public:
  V() : begin_(0, 0, 0), end_(begin_) {}
  V(float x, float y, float z) : begin_(0, 0, 0), end_(x, y, z) {}
  V(float x, float y, float z, float p, float q, float m)
      : begin_(x, y, z), end_(p, q, m) {}
  V(P begin, P end) : begin_(begin), end_(end) {}
  P& Begin() { return begin_; }
  P& End() { return end_; }

  friend bool operator==(const V& lhs, const V& rhs) {
    return lhs.begin_ == rhs.begin_ && lhs.end_ == rhs.end_;
  }

  friend V operator+(V lhs, const V& rhs) {
    // Под действием rhs изменится только lhs.end_.
    lhs.end_ = lhs.end_ + (rhs.end_ - rhs.begin_);
    return lhs;
  }

  friend V operator/(V lhs, const int factor) {
    lhs.end_.x(lhs.begin_.x() +
               std::fabs(lhs.end_.x() - lhs.begin_.x()) / factor);
    lhs.end_.y(lhs.begin_.y() +
               std::fabs(lhs.end_.y() - lhs.begin_.y()) / factor);
    return lhs;
  }

  friend V operator>>(V lhs, const float fraction) {
    const float dx = (lhs.End().x() - lhs.Begin().x()) * fraction;
    const float dy = (lhs.End().y() - lhs.Begin().y()) * fraction;
    const float dz = (lhs.End().z() - lhs.Begin().z()) * fraction;
    lhs.Begin().x(lhs.Begin().x() + dx);
    lhs.Begin().y(lhs.Begin().y() + dy);
    lhs.Begin().z(lhs.Begin().z() + dz);
    lhs.End().x(lhs.Begin().x() + dx);
    lhs.End().y(lhs.Begin().y() + dy);
    lhs.End().z(lhs.Begin().z() + dz);
    return lhs;
  }

 private:
};

// Three state bool: 1, 0, -1.
class tribool {
  int state_;

 public:
  tribool(int i = 0) : state_{i > 0 ? 1 : (i < 0 ? -1 : 0)} {}
  operator int() const { return state_ ? (state_ > 0 ? 1 : -1) : 0; }
  tribool& operator++() {
    state_ = state_ < 0 ? 0 : 1;
    return *this;
  }
  tribool& operator--() {
    state_ = state_ > 0 ? 0 : -1;
    return *this;
  }
};
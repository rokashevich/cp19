#pragma once
#include <cmath>
#include <iostream>

class P {
 public:
  float x;
  float y;
  float z;

  P() : x(0), y(0), z(0) {}
  P(float x, float y, float z) : x(x), y(y), z(z) {}

  float DistanceToSquared(P point) {
    return pow(x - point.x, 2) + pow(y - point.y, 2) + pow(z - point.z, 2);
  }

  float DistanceTo(P point) { return sqrt(DistanceToSquared(point)); }

  P& operator+=(const P& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  friend bool operator==(const P& lhs, const P& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
  }

  friend P operator+(P lhs, const P& rhs) {
    return P{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
  }

  friend P operator-(P lhs, const P& rhs) {
    return P{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
  }

  friend std::ostream& operator<<(std::ostream& os, const P& rhs) {
    os << rhs.x << " " << rhs.y << " " << rhs.z;
    return os;
  }
};

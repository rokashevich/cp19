#pragma once
#include <cmath>
#include <iostream>

class Point {
 public:
  float x;
  float y;
  float z;

  Point() : x(0), y(0), z(0) {}
  Point(float x, float y, float z) : x(x), y(y), z(z) {}

  float DistanceToSquared(Point point) {
    return pow(x - point.x, 2) + pow(y - point.y, 2) + pow(z - point.z, 2);
  }

  float DistanceTo(Point point) { return sqrt(DistanceToSquared(point)); }

  Point& operator+=(const Point& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  friend bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
  }

  friend Point operator+(Point lhs, const Point& rhs) {
    return Point{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
  }

  friend Point operator-(Point lhs, const Point& rhs) {
    return Point{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
  }

  friend std::ostream& operator<<(std::ostream& os, const Point& rhs) {
    os << rhs.x << " " << rhs.y << " " << rhs.z;
    return os;
  }
};

using P = Point;

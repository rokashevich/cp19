#pragma once
#include <cmath>

class Point {
 public:
  float x;
  float y;
  float z;

  Point() : x(0), y(0), z(0) {}
  Point(float x, float y, float z) : x(x), y(y), z(z) {}

  float DistanceToSquared(Point point) {
	return pow(x-point.x, 2) + pow(y-point.y, 2) + pow(z-point.z, 2);
  }

  float DistanceTo(Point point) { return sqrt(DistanceToSquared(point)); }

  friend Point operator+(Point lhs, const Point& rhs) {
    return Point{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
  }

  friend Point operator-(Point lhs, const Point& rhs) {
    return Point{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
  }
};

using P = Point;

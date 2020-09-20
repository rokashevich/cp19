#pragma once
#include <cmath>

class Point {
 public:
  float x;
  float y;
  float z;
  
  float DistanceToSquared(Point point) {
	return pow(x-point.x, 2) + pow(y-point.y, 2) + pow(z-point.z, 2);
  }
  
  float DistanceTo(Point point) {
	return sqrt(DistanceToSquared(point));
  }
  
};

using P = Point;

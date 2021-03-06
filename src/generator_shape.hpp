#pragma once

#include <vector>

class ShapeGenerator
{
 public:
  ShapeGenerator() = delete;
  static std::vector<float> Cuboid(float width, float height, float depth);
  static std::vector<float> Icosphere();
  static std::vector<float> Tube();
};

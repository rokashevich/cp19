#include "object.hpp"

#include <cassert>

#include "helpers/misc.hpp"
#include "helpers/shapegenerator.hpp"

const std::vector<float> O::shape_vertices_ =
    ShapeGenerator::Cuboid(0.02, 0.02, 0.02);
int O::ShapeVerticesCount() { return O::shape_vertices_.size(); }
int O::ShapeBytesCount() {
  assert(O::shape_vertices_.size() > 0);
  return O::ShapeVerticesCount() * sizeof(O::shape_vertices_.at(0));
}
const float *O::ShapeData() { return O::shape_vertices_.data(); }

Object::Object(P position, P direction, int weight)
    : v_(position, direction), weight_(weight) {}

O::O(P position, P direction, int diameter)
    : Object(position, direction,
             constants::pi * diameter * diameter * diameter / 6),
      diameter_(diameter) {}

O::~O() {}

void O::RenderParameters(std::vector<float> &p) {}

const std::vector<float> N::shape_vertices_ = ShapeGenerator::Cuboid(
    0.05 / constants::golden_ratio, 0.05, 0.05 / constants::golden_ratio);
int N::ShapeVerticesCount() { return N::shape_vertices_.size(); }
int N::ShapeBytesCount() {
  assert(N::shape_vertices_.size() > 0);
  return N::ShapeVerticesCount() * sizeof(N::shape_vertices_.at(0));
}
const float *N::ShapeData() { return N::shape_vertices_.data(); }

N::N(P position, P direction, int head, int body, int hands, int legs, int guns)
    : Object(position, direction, constants::pi * head * head * head / 6),
      head_(head),
      body_(body),
      hands_(hands),
      legs_(legs),
      guns_(guns){};

N::~N(){};

void N::RenderParameters(std::vector<float> &p) {}

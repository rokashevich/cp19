#include "object.hpp"

#include <cassert>

#include "helpers/misc.hpp"
#include "helpers/shapegenerator.hpp"

Object::Object(P position, P direction, float weight)
    : v_(position, direction), weight_(weight) {}

const std::vector<float> O::shape_vertices_ = ShapeGenerator::Icosphere();

O::O(P position, P direction, float diameter, float density)
    : Object(position, direction,
             density * constants::pi * diameter * diameter * diameter / 6),
      diameter_(diameter) {}

O::~O() {}

int O::ShapeVerticesCount() { return O::shape_vertices_.size(); }

int O::ShapeBytesCount() {
  assert(O::shape_vertices_.size() > 0);
  return O::ShapeVerticesCount() * sizeof(O::shape_vertices_.at(0));
}

const float *O::ShapeData() { return O::shape_vertices_.data(); }

void O::RenderParameters(std::vector<float> &p) {}

const std::vector<float> N::shape_vertices_ =
    ShapeGenerator::Cuboid(1, constants::golden_ratio, 1);

N::N(P position, P direction, int legs, int body, int head, int arms, int gun)
    : Object(position, direction, legs + body + head + arms),
      legs_(legs),
      body_(body),
      head_(head),
      arms_(arms),
      gun_(gun){};

N::~N(){};

int N::ShapeVerticesCount() { return N::shape_vertices_.size(); }

int N::ShapeBytesCount() {
  assert(N::shape_vertices_.size() > 0);
  return N::ShapeVerticesCount() * sizeof(N::shape_vertices_.at(0));
}
const float *N::ShapeData() { return N::shape_vertices_.data(); }

void N::RenderParameters(std::vector<float> &p) {}

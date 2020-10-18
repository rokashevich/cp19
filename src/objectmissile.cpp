#include "objectmissile.hpp"

#include <cassert>

#include "helpers/misc.hpp"
#include "helpers/shapegenerator.hpp"
const std::vector<float> ObjectMissile::shape_vertices_ =
    ShapeGenerator::Icosphere();

ObjectMissile::ObjectMissile(P position, P direction, float diameter,
                             float density)
    : Object(position, direction,
             density * constants::pi * diameter * diameter * diameter / 6),
      diameter_(diameter) {}

ObjectMissile::~ObjectMissile() {}

int ObjectMissile::ShapeVerticesCount() {
  return ObjectMissile::shape_vertices_.size();
}

int ObjectMissile::ShapeBytesCount() {
  assert(ObjectMissile::shape_vertices_.size() > 0);
  return ObjectMissile::ShapeVerticesCount() *
         sizeof(ObjectMissile::shape_vertices_.at(0));
}

const float *ObjectMissile::ShapeData() {
  return ObjectMissile::shape_vertices_.data();
}

void ObjectMissile::RenderParameters(std::vector<float> &p) {}

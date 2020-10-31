#include "object_missile.hpp"

#include <cassert>

#include "constants.hpp"

const std::vector<float> ObjectMissile::vertices_buffer_ =
    ShapeGenerator::Icosphere();

ObjectMissile::ObjectMissile(P position, P direction, float diameter)
    : Object(position, direction,
             constants::pi * diameter * diameter * diameter / 6),
      diameter_(diameter) {}

ObjectMissile::~ObjectMissile() {}

const float* ObjectMissile::ShapeVerticesBuffer() {
  return ObjectMissile::vertices_buffer_.data();
}

std::size_t ObjectMissile::SizeofShapeVerticesBuffer() {
  return sizeof(float) * ObjectMissile::vertices_buffer_.size();
}

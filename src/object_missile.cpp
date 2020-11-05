#include "object_missile.hpp"

#include <cassert>

#include "constants.hpp"

const std::vector<float> ObjectMissile::vertices_buffer_ =
    ShapeGenerator::Icosphere();

ObjectMissile::ObjectMissile(Vec v, float diameter)
    : Object(v, constants::pi * diameter * diameter * diameter / 6),
      diameter_(diameter) {}

ObjectMissile::~ObjectMissile() {}

const std::vector<float>* ObjectMissile::ShapeVerticesBuffer() {
  return &ObjectMissile::vertices_buffer_;
}

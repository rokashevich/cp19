#include "object_missile.hpp"

#include <cassert>

#include "constants.hpp"

const std::vector<float> ObjectMissile::vertices_buffer_ =
    ShapeGenerator::Icosphere();

const int ObjectMissile::num_vertices_ =
    ObjectMissile::vertices_buffer_.size() / 3;

ObjectMissile::ObjectMissile(P position, P direction, float diameter)
    : Object(position, direction,
             constants::pi * diameter * diameter * diameter / 6),
      diameter_(diameter) {}

ObjectMissile::~ObjectMissile() {}

const float* ObjectMissile::VerticesBuffer() {
  return ObjectMissile::vertices_buffer_.data();
}

std::size_t ObjectMissile::SizeofVerticesBuffer() {
  return sizeof(float) * ObjectMissile::vertices_buffer_.size();
}

int ObjectMissile::NumVertices() { return ObjectMissile::num_vertices_; }

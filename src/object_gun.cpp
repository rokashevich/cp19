#include "object_gun.hpp"

#include <cassert>

#include "constants.hpp"

const std::vector<float> ObjectGun::vertices_buffer_ = ShapeGenerator::Tube();

ObjectGun::~ObjectGun() {}

const std::vector<float>* ObjectGun::ShapeVerticesBuffer() {
  return &ObjectGun::vertices_buffer_;
}

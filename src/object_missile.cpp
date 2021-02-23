#include "object_missile.hpp"

#include <cassert>

#include "constants.hpp"
#include "pixel_missile.hpp"
#include "vertex_missile.hpp"
template <>
const ObjectsStaticInfo Shape<ObjectMissile>::objects_static_info_{
    ShapeGenerator::Icosphere(), vertex_missile, pixel_missile};

const std::vector<float> ObjectMissile::vertices_buffer_ =
    ShapeGenerator::Icosphere();

ObjectMissile::ObjectMissile(Vec v, float diameter)
    : Object(v, constants::pi * diameter * diameter * diameter / 6 * 1000),
      diameter_(diameter) {}

ObjectMissile::~ObjectMissile() {}

const std::vector<float>* ObjectMissile::ShapeVerticesBuffer() {
  return &ObjectMissile::vertices_buffer_;
}

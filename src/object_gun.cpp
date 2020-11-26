#include "object_gun.hpp"

#include <cassert>

#include "constants.hpp"
#include "pixel_gun.hpp"
#include "vertex_gun.hpp"
template <>
const ObjectsStaticInfo Shape<ObjectGun>::objects_static_info_{
    ShapeGenerator::Tube(), vertex_gun, pixel_gun};

const std::vector<float> ObjectGun::vertices_buffer_ = ShapeGenerator::Tube();

ObjectGun::~ObjectGun() {}

const std::vector<float>* ObjectGun::ShapeVerticesBuffer() {
  return &ObjectGun::vertices_buffer_;
}

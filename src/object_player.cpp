#include "object_player.hpp"

#include <cassert>

#include "constants.hpp"
#include "generator_shape.hpp"
#include "pixel_player.hpp"
#include "vertex_player.hpp"
template <>
const ObjectsStaticInfo Shape<ObjectPlayer>::objects_static_info_{
    ShapeGenerator::Cuboid(1, 1, 1), vertex_player, pixel_player};

const std::vector<float> ObjectPlayer::vertices_buffer_ =
    ShapeGenerator::Cuboid(1, 1, 1);

ObjectPlayer::ObjectPlayer(Vec v, int legs, int body, int head, int arms)
    : Object(v, legs + body + head + arms),
      legs_(legs),
      body_(body),
      head_(head),
      arms_(arms) {
  Object::coords_params_.resize(2);
}
ObjectPlayer::~ObjectPlayer(){};

const std::vector<float>* ObjectPlayer::ShapeVerticesBuffer() {
  return &ObjectPlayer::vertices_buffer_;
}

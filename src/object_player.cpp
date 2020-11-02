#include "object_player.hpp"

#include <cassert>

#include "constants.hpp"
#include "generator_shape.hpp"

const std::vector<float> ObjectPlayer::vertices_buffer_ =
    ShapeGenerator::Cuboid(1, 2, 1);

ObjectPlayer::ObjectPlayer(int legs, int body, int head, int arms)
    : Object(legs + body + head + arms),
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

#include "object_player.hpp"

#include <cassert>

#include "constants.hpp"
#include "generator_shape.hpp"

const std::vector<float> ObjectPlayer::vertices_buffer_ =
    ShapeGenerator::Cuboid(1, constants::golden_ratio, 1);

const int ObjectPlayer::num_vertices_ =
    ObjectPlayer::vertices_buffer_.size() / 3;

ObjectPlayer::ObjectPlayer(P position, P direction, int legs, int body,
                           int head, int arms, int gun)
    : Object(position, direction, legs + body + head + arms),
      legs_(legs),
      body_(body),
      head_(head),
      arms_(arms),
      gun_(gun){};
ObjectPlayer::~ObjectPlayer(){};

const float* ObjectPlayer::VerticesBuffer() {
  return ObjectPlayer::vertices_buffer_.data();
}

std::size_t ObjectPlayer::SizeofVerticesBuffer() {
  return sizeof(float) * ObjectPlayer::vertices_buffer_.size();
}

int ObjectPlayer::NumVertices() { return ObjectPlayer::num_vertices_; }

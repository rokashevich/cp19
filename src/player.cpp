#include "player.hpp"

#include <cassert>

#include "constants.hpp"
#include "generator_shape.hpp"
#include "pixel_player.hpp"
#include "vertex_player.hpp"
template <>
const ShapeInfo Shape<Player>::objects_static_info_{
    ShapeGenerator::Cuboid(1, 1, 1), vertex_player, pixel_player};

const std::vector<float> Player::vertices_buffer_ =
    ShapeGenerator::Cuboid(1, 1, 1);

Player::Player(glm::vec3 coords) : Object(coords) {}

// TODO OLD
Player::Player(Vec v, int legs, int body, int head, int arms)
    : Object(v, legs + body + head + arms),
      legs_(legs),
      body_(body),
      head_(head),
      arms_(arms) {
  // Object::offsets_old_.resize(2);
}
Player::~Player(){};

const std::vector<float>* Player::ShapeVerticesBuffer() {
  return &Player::vertices_buffer_;
}

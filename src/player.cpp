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

Player::Player(glm::vec3 coords, glm::vec3 angles) : Object(coords, angles) {
  coords_.resize(Object::num_coords_ *
                 (Object::num_shapes_ + Player::num_shapes_));
  angles_.resize(Object::num_angles_ *
                 (Object::num_shapes_ + Player::num_shapes_));
  params_.resize(Object::num_params_ *
                 (Object::num_shapes_ + Player::num_shapes_));
}
int Player::NumInstances() { return Player::num_shapes_; }
void Player::Step() {
  coords_.at(3) = 1.0f;
  coords_.at(4) = 0.0f;
  coords_.at(5) = 0.0f;

  coords_.at(6) = -1.0f;
  coords_.at(7) = 0.0f;
  coords_.at(8) = 0.0f;

  coords_.at(9) = 0.0f;
  coords_.at(10) = 1.0f;
  coords_.at(11) = 0.0f;

  coords_.at(12) = 0.0f;
  coords_.at(13) = -1.0f;
  coords_.at(14) = 0.0f;
}

//{
//   Object::offsets_old_.at(0).at(0) = 0;
//   Object::offsets_old_.at(0).at(1) = 0;
//   // Object::offsets_.at(0).at(3) = 1;

//   Object::offsets_old_.at(1).at(1) = -1.25;
//   Object::offsets_old_.at(1).at(2) = 0.25;
//   // Object::offsets_.at(1).at(3) = 1.5;
// }

// TODO OLD
// Player::Player(Vec v, int legs, int body, int head, int arms)
//     : Object(v, legs + body + head + arms),
//       legs_(legs),
//       body_(body),
//       head_(head),
//       arms_(arms) {
//   // Object::offsets_old_.resize(2);
// }
Player::~Player(){};

const std::vector<float>* Player::ShapeVerticesBuffer() {
  return &Player::vertices_buffer_;
}

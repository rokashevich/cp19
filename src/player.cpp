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
  coords_.resize(Object::num_coords * Player::num_instances_);
  angles_.resize(Object::num_angles * Player::num_instances_);
  params_.resize(Object::num_params * Player::num_instances_);
}
int Player::NumInstances() { return Player::num_instances_; }
void Player::Step() {  // Тестовая анимация.
  static size_t step{0};
  std::vector<int> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  const auto k{5.0f / a[step]};
  coords_.at(0) = k + 1;
  coords_.at(3) = k + 2;
  coords_.at(6) = k + 3;
  coords_.at(9) = k + 4;
  coords_.at(12) = k + 5;
  coords_.at(15) = k + 6;
  coords_.at(18) = k + 7;
  coords_.at(21) = k + 8;
  ++step;
  if (step == a.size()) step = 0;
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

#include "player.hpp"

#include <cassert>

#include "animation.hpp"
#include "constants.hpp"
#include "generator_shape.hpp"
#include "pixel_player.hpp"
#include "vertex_player.hpp"

Player::Player(glm::vec3 c, glm::vec3 angles, int head, int body, int arms,
               int legs)
    : Object(c, angles) {}
int Player::NumInstances() { return Player::num_instances_; }
void Player::Step() {  // Тестовая анимация.

  // static bool animation_started = false;
  // if (!animation_started) {
  //   animation_.StartSequence(Animation::Sequence::kWalk);
  //   animation_started = true;
  // }

  // auto k = animation_.Next();
  // coords_.at(0) = k + 1;
  // coords_.at(3) = k + 2;
  // coords_.at(6) = k + 3;
  // coords_.at(9) = k + 4;
  // coords_.at(12) = k + 5;
  // coords_.at(15) = k + 6;
  // coords_.at(18) = k + 7;
  // coords_.at(21) = k + 8;
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
// Player::Player(V v, int legs, int body, int head, int arms)
//     : Object(v, legs + body + head + arms),
//       legs_(legs),
//       body_(body),
//       head_(head),
//       arms_(arms) {
//   // Object::offsets_old_.resize(2);
// }
Player::~Player(){};

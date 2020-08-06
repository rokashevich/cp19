#include "object.hpp"
#include "helpers/misc.hpp"

Object::Object(int weight, float x, float y, float z)
    : weight_(weight), x_(x), y_(y), z_(z) {}

Missile::Missile(int diameter, float x, float y, float z)
    : Object(constants::pi * diameter * diameter * diameter / 6, x, y, z),
      diameter_(diameter) {}

Missile::~Missile() {}

Player::Player(int head, int body, int hands, int legs, int guns, float x,
               float y, float z)
    : Object(0, x, y, z),
      head_(head),
      body_(body),
      hands_(hands),
      legs_(legs),
      guns_(guns){};

Player::~Player(){};

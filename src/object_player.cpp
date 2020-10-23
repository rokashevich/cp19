#include "object_player.hpp"

#include <cassert>

#include "constants.hpp"
#include "generator_shape.hpp"
template <>
const std::vector<Shape<ObjectPlayer>::t> Shape<ObjectPlayer>::data{
    ShapeGenerator::Cuboid(1, constants::golden_ratio, 1)};
ObjectPlayer::ObjectPlayer(P position, P direction, int legs, int body,
                           int head, int arms, int gun)
    : Object(position, direction, legs + body + head + arms),
      legs_(legs),
      body_(body),
      head_(head),
      arms_(arms),
      gun_(gun){};
ObjectPlayer::~ObjectPlayer(){};
void ObjectPlayer::RenderParameters(std::vector<float> &p) {}

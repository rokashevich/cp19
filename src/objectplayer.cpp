#include "objectplayer.hpp"

#include <cassert>

#include "helpers/misc.hpp"
#include "helpers/shapegenerator.hpp"

const std::vector<float> ObjectPlayer::shape_vertices_ =
    ShapeGenerator::Cuboid(1, constants::golden_ratio, 1);

ObjectPlayer::ObjectPlayer(P position, P direction, int legs, int body,
                           int head, int arms, int gun)
    : Object(position, direction, legs + body + head + arms),
      legs_(legs),
      body_(body),
      head_(head),
      arms_(arms),
      gun_(gun){};

ObjectPlayer::~ObjectPlayer(){};

int ObjectPlayer::ShapeVerticesCount() {
  return ObjectPlayer::shape_vertices_.size();
}

int ObjectPlayer::ShapeBytesCount() {
  assert(ObjectPlayer::shape_vertices_.size() > 0);
  return ObjectPlayer::ShapeVerticesCount() *
         sizeof(ObjectPlayer::shape_vertices_.at(0));
}
const float *ObjectPlayer::ShapeData() {
  return ObjectPlayer::shape_vertices_.data();
}

void ObjectPlayer::RenderParameters(std::vector<float> &p) {}

#include "object_wall.hpp"

#include "constants.hpp"

const std::vector<float> ObjectWall::vertices_buffer_ =
    ShapeGenerator::Cuboid(2.8, 2.8, 0.1);

ObjectWall::ObjectWall(P position, int orientation_helath)
    : Object(position, position, 0), health_(orientation_helath) {}

ObjectWall::~ObjectWall() {}

const std::vector<float>* ObjectWall::ShapeVerticesBuffer() {
  return &ObjectWall::vertices_buffer_;
}

#include "object_wall.hpp"

#include "constants.hpp"

const std::vector<float> ObjectWall::vertices_buffer_ =
    ShapeGenerator::Cuboid(2.8, 2.8, 0.1);

const int ObjectWall::num_vertices_ = ObjectWall::vertices_buffer_.size() / 3;

ObjectWall::ObjectWall(int helath)
    : Object(P{0, 0, 0}, P{0, 0, 0}, 0), health_(helath) {}

ObjectWall::~ObjectWall() {}

const float* ObjectWall::VerticesBuffer() {
  return ObjectWall::vertices_buffer_.data();
}

std::size_t ObjectWall::SizeofVerticesBuffer() {
  return sizeof(float) * ObjectWall::vertices_buffer_.size();
}

int ObjectWall::NumVertices() { return ObjectWall::num_vertices_; }

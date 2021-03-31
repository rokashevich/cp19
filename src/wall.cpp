#include "wall.hpp"

#include <glm/vec3.hpp>

#include "constants.hpp"
#include "pixel_wall.hpp"
#include "vertex_wall.hpp"
template <>
const ShapeInfo Shape<Wall>::objects_static_info_{
    ShapeGenerator::Cuboid(2.9, 2.9, 0.05), vertex_wall, pixel_wall};

const std::vector<float> Wall::vertices_buffer_ =
    ShapeGenerator::Cuboid(2.9, 2.9, 0.05);

Wall::Wall(Vec v, int orientation_helath, glm::vec3 angles, glm::vec3 params)
    : Object(v, 0), health_(orientation_helath) {}

Wall::~Wall() {}

const std::vector<float>* Wall::ShapeVerticesBuffer() {
  return &Wall::vertices_buffer_;
}

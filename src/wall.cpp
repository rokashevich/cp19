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

Wall::Wall(glm::vec3 coords, glm::vec3 angles) : Object(coords, angles) {}
Wall::~Wall() {}

// TODO OLD
// Wall::Wall(V v, glm::vec3 angles, glm::vec3 params)
//     : Object(v, 0, angles, params) {}

const std::vector<float>* Wall::ShapeVerticesBuffer() {
  return &Wall::vertices_buffer_;
}

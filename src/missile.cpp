#include "missile.hpp"

#include <cassert>

#include "constants.hpp"
#include "pixel_missile.hpp"
#include "vertex_missile.hpp"
template <>
const ShapeInfo Shape<Missile>::objects_static_info_{
    ShapeGenerator::Icosphere(), vertex_missile, pixel_missile};

const std::vector<float> Missile::vertices_buffer_ =
    ShapeGenerator::Icosphere();

// Missile::Missile(V v, float diameter)
//     : Object(v, constants::pi * diameter * diameter * diameter / 6 * 1000),
//       diameter_(diameter) {}

Missile::~Missile() {}

const std::vector<float>* Missile::ShapeVerticesBuffer() {
  return &Missile::vertices_buffer_;
}

#include "object_wall.hpp"

#include "constants.hpp"

template <>
const std::vector<Shape<ObjectWall>::t> Shape<ObjectWall>::data_{
    ShapeGenerator::Cuboid(constants::wall_width, constants::wall_height,
                           constants::wall_thickness)};

ObjectWall::ObjectWall(int helath)
    : Object(P{0, 0, 0}, P{0, 0, 0}, 0), health_(helath) {}

ObjectWall::~ObjectWall() {}

void ObjectWall::RenderParameters(std::vector<float> &p) {}

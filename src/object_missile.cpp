#include "object_missile.hpp"

#include "constants.hpp"

template <>
const std::vector<Shape<ObjectMissile>::t> Shape<ObjectMissile>::data{
    ShapeGenerator::Icosphere()};

ObjectMissile::ObjectMissile(P position, P direction, float diameter,
                             float density)
    : Object(position, direction,
             density * constants::pi * diameter * diameter * diameter / 6),
      diameter_(diameter),
      density_(density) {}

ObjectMissile::~ObjectMissile() {}

void ObjectMissile::RenderParameters(std::vector<float> &p) {}

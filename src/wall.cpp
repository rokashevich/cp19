#include "wall.hpp"

#include <glm/vec3.hpp>

#include "constants.hpp"
#include "pixel_wall.hpp"
#include "vertex_wall.hpp"

Wall::Wall(glm::vec3 coords, glm::vec3 angles) : Object(coords, angles) {}
Wall::~Wall() {}

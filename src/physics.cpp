#include "physics.hpp"

#include <iostream>

int Physics::target_fps_{20};
int Physics::step_ticks_{1000 / Physics::target_fps_};

#include "physics.hpp"

#include <iostream>

int Physics::target_fps_{20};
int Physics::frame_ms_{1000 / Physics::target_fps_};
float Physics::frame_fraction_sec_{1.0f / Physics::target_fps_};

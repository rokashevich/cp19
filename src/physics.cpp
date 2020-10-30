#include "physics.hpp"

#include <iostream>

int Physics::target_fps_{20};
int Physics::step_ticks_{1000 / Physics::target_fps_};

void Physics::Step() {
  // Сдвигаем таймер на прошедшее с предыдущего шага время.
  Timer::Step(Physics::step_ticks_);

  for (auto const &a : group_) {
    ObjectGroupContainer *group = a.second;
    if (!group->is_dynamic) {
      if (group->is_state_changed == yes)
        group->is_state_changed = yesnextno;
      else if (group->is_state_changed == yesnextno)
        group->is_state_changed = no;
    }
    int i = -1;
    for (auto &object : group->objects) {
      group->coords_params_buffer_.at(++i) = object->x();
      group->coords_params_buffer_.at(++i) = object->y();
      group->coords_params_buffer_.at(++i) = object->z();
      group->coords_params_buffer_.at(++i) = object->w();
    }
    // std::cout << i << std::endl;
  }
}

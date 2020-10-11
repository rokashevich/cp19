#include "physics.hpp"

#include <iostream>

int Physics::target_fps_{20};
int Physics::step_ticks_{1000 / Physics::target_fps_};

std::list<Object *> Physics::is_;
std::list<Object *> Physics::os_;
std::list<Object *> Physics::ns_;

std::vector<float> Physics::render_parameters_i;
std::vector<float> Physics::render_parameters_o;
std::vector<float> Physics::render_parameters_n;

void Physics::AddI(Object *object) { Physics::is_.push_back(object); }
void Physics::AddO(Object *object) { Physics::os_.push_back(object); }
void Physics::AddN(Object *object) { Physics::ns_.push_back(object); }

const Physics::RenderParameters Physics::RenderParametersO() {
  Physics::render_parameters_o.clear();
  for (const auto &o : Physics::os_) {
    Physics::render_parameters_o.push_back(o->x());
    Physics::render_parameters_o.push_back(o->y());
    Physics::render_parameters_o.push_back(o->z());
    Physics::render_parameters_o.push_back(o->weight_);
  }
  return RenderParameters{Physics::render_parameters_o.data(),
                          static_cast<int>(Physics::render_parameters_o.size()),
                          static_cast<int>(os_.size())};
}

void Physics::Step() {
  // Сдвигаем таймер на прошедшее с предыдущего шага время.
  Timer::Step(Physics::step_ticks_);

  // Смещаем все объекты в соответствие с их векторами и таймером.
  for (auto const &o : Physics::os_) {
    static float start_y = o->y();
    static float sign = 1;
    if (o->y() > 1 && sign > 0)
      sign = -1;
    else if (o->y() < -1 && sign < 0)
      sign = 1;
    o->y() = o->y() + 0.05 * sign;
  }
}

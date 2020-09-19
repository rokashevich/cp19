#include "physics.hpp"

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
  Timer::Step();

  // Смещаем все объекты в соответствие с их векторами и таймером.
}

int Physics::Delta() { return Timer::Delta(); }

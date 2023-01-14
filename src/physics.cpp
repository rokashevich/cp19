#include "physics.hpp"

#include <functional>
#include <iostream>
#include <tuple>
#include <vector>
int Physics::target_fps_{20};
int Physics::frame_ms_{1000 / Physics::target_fps_};
float Physics::frame_fraction_sec_{1.0f / Physics::target_fps_};
void Physics::AddObject(int key, Object* o) {
  // TODO Переделать на фиксированный массив векторов!
  Group* g = FindGroup(key);
  g->objects.push_back(o);

  // Добавили объект - добавляем и кол-во фигур в одном объекте
  // для последующей инстансированной отрисовки.
  g->num_instances += o->NumInstances();

  g->coords_.resize(g->coords_.size() + Object::num_coords * o->NumInstances());
  g->angles_.resize(g->angles_.size() + Object::num_angles * o->NumInstances());
  g->params_.resize(g->params_.size() + Object::num_params * o->NumInstances());

  const float* coords_data{g->coords_.data()};
  const float* angles_data{g->angles_.data()};
  const float* params_data{g->params_.data()};
  size_t coords_size{g->coords_.size()};
  size_t angles_size{g->angles_.size()};
  size_t params_size{g->params_.size()};

  ObjectsChangedCallback(key, g->num_instances, coords_data, coords_size,
                         angles_data, angles_size, params_data, params_size);
}
void Physics::Step() {
  // Change positions of all moving objects.
  // TODO Collision detection!!!

  for (auto& g : groups_) {
    Group& group = *g.second;
    for (auto& object : group.objects) {
      object->coords[2] += object->backward_forward;
      object->coords[0] -= object->left_right;
      object->coords[1] += object->up_down;
      object->backward_forward = 0;
      object->left_right = 0;
      object->up_down = 0;
      //   for (auto const& object : group_container->objects) {
      //     object->Step();
      //     // todo физику пока отменяем
      //     // const V g{0, 0, 0, 0, -0.02, 0};  // ускор свобод падения
      //     // if (object->Weight() > 0) {  // динамический объект
      //     //  object->V() = object->V() + g;
      //     //}
      //   }
    }

    auto key = g.first;
    // int i = 0;

    // Копируем параметры однотипных объектов в массивы для инстансированного
    // рендеринга.
    auto coords_offset{0};
    auto angles_offset{0};
    auto params_offset{0};
    for (const auto& object : group.objects) {
      // if (key == 1) {
      //   std::cout << g->coords_.size() << ":" << g->coords_.at(0) <<
      //   std::endl;
      // }
      auto coords_begin{group.coords_.begin() + coords_offset};
      auto angles_begin{group.angles_.begin() + angles_offset};
      auto params_begin{group.params_.begin() + angles_offset};
      std::copy(object->Coords().begin(), object->Coords().end(), coords_begin);
      std::copy(object->Angles().begin(), object->Angles().end(), angles_begin);
      std::copy(object->Params().begin(), object->Params().end(), params_begin);
      coords_offset += object->Coords().size();
      angles_offset += object->Angles().size();
      params_offset += object->Params().size();
    }
  }

  // Sleep enough to have same duration since last step
  Timer::SleepAlign(Physics::frame_ms_);
}
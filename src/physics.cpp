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
  // Сдвигаем таймер на прошедшее с предыдущего шага время.
  Timer::Step(Physics::frame_ms_);

  // Первая итерация по всем объектам - смещаем на запрашиваему величину.
  for (auto& g : groups_) {
    Group& group = *g.second;
    for (auto& object : group.objects) {
      // object->Step();
    }
    //   for (auto const& object : group_container->objects) {
    //     object->Step();
    //     // todo физику пока отменяем
    //     // const Vec g{0, 0, 0, 0, -0.02, 0};  // ускор свобод падения
    //     // if (object->Weight() > 0) {  // динамический объект
    //     //  object->V() = object->V() + g;
    //     //}
    //   }
  }
  // Вторая итерация - разрешаем коллизии.
  for (const auto& key_group_pair : groups_) {
    Group* g = key_group_pair.second;
    // int i = 0;

    auto coords_offset{0};
    auto angles_offset{0};
    auto params_offset{0};
    for (const auto& o : g->objects) {
      // const P b = object->V().Begin();
      //       object->V() = object->V() >> 0.1;
      auto coords_begin{g->coords_.begin() + coords_offset};
      auto angles_begin{g->angles_.begin() + angles_offset};
      auto params_begin{g->params_.begin() + angles_offset};
      std::copy(o->Coords().begin(), o->Coords().end(), coords_begin);
      std::copy(o->Angles().begin(), o->Angles().end(), angles_begin);
      std::copy(o->Params().begin(), o->Params().end(), params_begin);
      coords_offset += o->Coords().size();
      angles_offset += o->Angles().size();
      params_offset += o->Params().size();

      // glm::vec3 m = o->ImpulsePolite();
      // if (m.x || m.y || m.z)
      // std::cout << "move:" << m.x << " " << m.y << " " << m.z << '\n';

      // g->coords_.at(i + 0) = o->Coords().at(0);
      // g->coords_.at(i + 1) = o->Coords().at(1);
      // g->coords_.at(i + 2) = o->Coords().at(2);

      // g->angles_.at(i + 0) = o->Angles().at(0);
      // g->angles_.at(i + 1) = o->Angles().at(1);
      // g->angles_.at(i + 2) = o->Angles().at(2);

      // g->params_.at(i + 0) = o->Params().at(0);
      // g->params_.at(i + 1) = o->Params().at(1);
      // g->params_.at(i + 2) = o->Params().at(2);

      // i += 3;
    }
  }
}
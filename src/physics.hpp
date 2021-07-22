#pragma once

#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <list>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "object.hpp"
#include "timer.hpp"

class Physics : protected Timer {
  // Контейнер хранения информации по объектам одного вида.
  struct Group {
    int num_instances{};
    std::vector<Object*> objects;

    // Буфферы для шейдеров.
    // В шейдеры данные передаются в виде одномерных массивов floatов и
    // размеры этих массивов в байтах. Делается это каждый кадр!
    std::vector<float> coords_;
    std::vector<float> angles_;
    std::vector<float> params_;
  };
  std::unordered_map<int, Group*> groups_;
  std::function<void(int, int, const std::vector<float>&,
                     const std::vector<float>&, const std::vector<float>&)>
      ObjectsChangedCallback;

 public:
  Physics(
      std::function<void(int, int, const std::vector<float>&,
                         const std::vector<float>&, const std::vector<float>&)>
          objects_changed_callback) {
    ObjectsChangedCallback = objects_changed_callback;
  }
  void RegisterObject(int key) { groups_[key] = new Group; }

  // Регистрирует объект типа key.
  void AddObject(int key, Object* object) {
    // TODO Переделать на фиксированный массив векторов!
    Group* group = FindGroup(key);
    group->objects.push_back(object);

    // Добавили объект - добавляем и кол-во граней в одном объекте
    // для последующей инстансированной отрисовки.
    const auto current_size{group->coords_.size()};
    group->num_instances += object->NumInstances();

    // 3, 3 и 4 - это кол-во floatoв в coords, angles и params объекта.
    group->coords_.resize(current_size + 3 * object->NumInstances());
    group->angles_.resize(current_size + 3 * object->NumInstances());
    group->params_.resize(current_size + 4 * object->NumInstances());

    ObjectsChangedCallback(key, group->num_instances, (*group).coords_,
                           (*group).angles_, (*group).params_);
  }
  void Step() {
    // Сдвигаем таймер на прошедшее с предыдущего шага время.
    Timer::Step(Physics::frame_ms_);

    // Первая итерация по всем объектам - смещаем на запрашиваему величину.
    for (auto& g : groups_) {
      Group& group = *g.second;
      for (auto& object : group.objects) {
        object->Step();
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
      Group* group = key_group_pair.second;
      int i = 0;
      for (const auto& object : group->objects) {
        // todo физику пока отменяем
        // const Point b = object->V().Begin();
        //       object->V() = object->V() >> 0.1;

        group->coords_.at(i + 0) = object->Coords().at(0);
        group->coords_.at(i + 1) = object->Coords().at(1);
        group->coords_.at(i + 2) = object->Coords().at(2);

        group->angles_.at(i + 0) = object->Angles().at(0);
        group->angles_.at(i + 1) = object->Angles().at(1);
        group->angles_.at(i + 2) = object->Angles().at(2);

        group->params_.at(i + 0) = object->Params().at(0);
        group->params_.at(i + 1) = object->Params().at(1);
        group->params_.at(i + 2) = object->Params().at(2);

        i += 3;
      }
    }
  }

 private:
  Group* FindGroup(int key) {
    auto it = groups_.find(key);
    assert(it != groups_.end());
    return groups_.find(key)->second;
  }
  static int target_fps_;
  static int frame_ms_;
  static float frame_fraction_sec_;
};

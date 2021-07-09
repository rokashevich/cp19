#pragma once

#include <array>
#include <cassert>
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
    bool is_dynamic;
    Object* reference_object_;
    int num_shapes;
    std::vector<Object*> objects;

    // Буфферы для шейдеров.
    // В шейдеры данные передаются в виде одномерных массивов floatов и
    // размеры этих массивов в байтах. Делается это каждый кадр!
    std::vector<float> coords_;
    std::vector<float> angles_;
    std::vector<float> params_;
    std::size_t coords_size_;
    std::size_t angles_size_;
    std::size_t params_size_;
  };
  std::unordered_map<int, Group*> object_groups_;

 public:
  void SetupObject(int key, Object* reference_object) {
    assert(object_groups_.find(key) == object_groups_.end());
    auto container = new Group;
    container->reference_object_ = reference_object;
    container->num_shapes = 0;
    object_groups_[key] = container;
  }

  // Регистрирует объект типа key.
  void AddObject(int key, Object* object) {
    // TODO Переделать на фиксированный массив векторов!
    Group* group = FindGroupContainer(key);
    group->objects.push_back(object);

    // Добавили объект - добавляем и кол-во граней в одном объекте
    // для последующей инстансированной отрисовки.
    const auto current_size{group->coords_.size()};
    const auto shapes_in_object{group->reference_object_->Offsets().size()};
    group->num_shapes += shapes_in_object;

    // 3, 3 и 4 - это кол-во floatoв в coords, angles и params объекта.
    group->coords_.resize(current_size + 3 * shapes_in_object);
    group->coords_size_ = group->coords_.size() * sizeof(float);
    group->angles_.resize(current_size + 3 * shapes_in_object);
    group->angles_size_ = group->angles_.size() * sizeof(float);
    group->params_.resize(current_size + 3 * shapes_in_object);
    group->params_size_ = group->params_.size() * sizeof(float);
  }

  float* Coords(int key) {
    return object_groups_.find(key)->second->coords_.data();
  }

  std::size_t CoordsSize(int key) {
    return object_groups_.find(key)->second->coords_size_;
  }

  float* Angles(int key) {
    return object_groups_.find(key)->second->angles_.data();
  }

  std::size_t AnglesSize(int key) {
    return object_groups_.find(key)->second->angles_size_;
  }

  float* Params(int key) {
    return object_groups_.find(key)->second->params_.data();
  }

  std::size_t ParamsSize(int key) {
    return object_groups_.find(key)->second->params_size_;
  }

  int NumShapes(int key) {
    Group* group = FindGroupContainer(key);
    return group->num_shapes;
  }

  const float* ShapeVerticesBuffer(int key) {
    Group* container = FindGroupContainer(key);
    return container->reference_object_->ShapeVerticesBuffer()->data();
  }

  void Step() {
    // Сдвигаем таймер на прошедшее с предыдущего шага время.
    Timer::Step(Physics::frame_ms_);

    // Первая итерация по всем объектам - смещаем на запрашиваему величину.
    // for (auto const& key_group_pair : object_groups_) {
    //   Group* group_container = key_group_pair.second;
    //   for (auto const& object : group_container->objects) {
    //     object->Step();
    //     // todo физику пока отменяем
    //     // const Vec g{0, 0, 0, 0, -0.02, 0};  // ускор свобод падения
    //     // if (object->Weight() > 0) {  // динамический объект
    //     //  object->V() = object->V() + g;
    //     //}
    //   }
    // }
    // Вторая итерация - разрешаем коллизии.
    for (auto const& key_group_pair : object_groups_) {
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
  Group* FindGroupContainer(int key) {
    auto it = object_groups_.find(key);
    assert(it != object_groups_.end());
    return object_groups_.find(key)->second;
  }
  static int target_fps_;
  static int frame_ms_;
  static float frame_fraction_sec_;
};

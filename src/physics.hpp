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
  struct ObjectGroupContainer {
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
  std::unordered_map<int, ObjectGroupContainer*> object_groups_;

 public:
  void SetupObject(int key, Object* reference_object) {
    assert(object_groups_.find(key) == object_groups_.end());
    auto container = new ObjectGroupContainer;
    container->reference_object_ = reference_object;
    container->num_shapes = 0;
    object_groups_[key] = container;
  }

  void AddObject(int key, Object* object) {
    ObjectGroupContainer* group = FindGroupContainer(key);
    group->objects.push_back(object);

    const int current_size = group->coords_.size();
    const int shapes_in_object = group->reference_object_->Offsets().size();
    group->num_shapes += shapes_in_object;

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
    ObjectGroupContainer* group = FindGroupContainer(key);
    return group->num_shapes;
  }

  const float* ShapeVerticesBuffer(int key) {
    ObjectGroupContainer* container = FindGroupContainer(key);
    return container->reference_object_->ShapeVerticesBuffer()->data();
  }

  void Step() {
    // Сдвигаем таймер на прошедшее с предыдущего шага время.
    Timer::Step(Physics::frame_ms_);

    // Первая итерация по всем объектам - смещаем на запрашиваему величину.
    for (auto const& key_group_pair : object_groups_) {
      ObjectGroupContainer* group_container = key_group_pair.second;
      for (auto const& object : group_container->objects) {
        object->Step();
        // todo физику пока отменяем
        // const Vec g{0, 0, 0, 0, -0.02, 0};  // ускор свобод падения
        // if (object->Weight() > 0) {  // динамический объект
        //  object->V() = object->V() + g;
        //}
      }
    }
    // Вторая итерация - разрешаем коллизии.
    for (auto const& key_group_pair : object_groups_) {
      ObjectGroupContainer* group = key_group_pair.second;
      int i = -1;
      for (auto const& object : group->objects) {
        // todo физику пока отменяем
        // const Point b = object->V().Begin();
        //       object->V() = object->V() >> 0.1;
        const Point& coord = object->V().Begin();
        for (auto const& shape_coords_params : object->Offsets()) {
          group->coords_.at(++i) = coord.x + shape_coords_params.at(0);
          group->coords_.at(++i) = coord.y + shape_coords_params.at(1);
          group->coords_.at(++i) = coord.z + shape_coords_params.at(2);
          // group->coords_.at(++i) = shape_coords_params.at(3);
        }
      }
    }
  }

 private:
  ObjectGroupContainer* FindGroupContainer(int key) {
    auto it = object_groups_.find(key);
    assert(it != object_groups_.end());
    return object_groups_.find(key)->second;
  }
  static int target_fps_;
  static int frame_ms_;
  static float frame_fraction_sec_;
};

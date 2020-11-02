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
  struct ObjectContainer {
    Object* object;
    Vec v;
  };
  struct ObjectGroupContainer {
    bool is_dynamic;
    Object* reference_object_;
    int num_shapes;
    std::vector<ObjectContainer> objects;
    std::vector<float> coords_params_buffer_;
    std::size_t sizeof_coords_params_buffer_;
  };
  std::unordered_map<int, ObjectGroupContainer*> object_groups_;

 public:
  void SetupObject(int key, Object* reference_object, bool is_dynamic) {
    assert(object_groups_.find(key) == object_groups_.end());
    auto container = new ObjectGroupContainer;
    container->is_dynamic = is_dynamic;
    container->reference_object_ = reference_object;
    container->num_shapes = 0;
    object_groups_[key] = container;
  }

  void AddObject(int key, Object* object, Vec v) {
    ObjectGroupContainer* group = FindGroupContainer(key);
    group->objects.push_back(ObjectContainer{object, v});
    const int current_size = group->coords_params_buffer_.size();
    const int shapes_in_object =
        group->reference_object_->CoordsParams()->size();
    const int add_size = 4 * shapes_in_object;
    group->coords_params_buffer_.resize(current_size + add_size);
    group->sizeof_coords_params_buffer_ =
        group->coords_params_buffer_.size() * sizeof(float);
    group->num_shapes += shapes_in_object;
  }

  float* CoordsParamsBuffer(int key) {
    auto it = object_groups_.find(key);
    assert(it != object_groups_.end());
    ObjectGroupContainer* contaier = object_groups_.find(key)->second;
    return contaier->coords_params_buffer_.data();
  }

  std::size_t SizeofCoordsParamsBuffer(int key) {
    auto it = object_groups_.find(key);
    assert(it != object_groups_.end());
    const ObjectGroupContainer* contaier = object_groups_.find(key)->second;
    return contaier->sizeof_coords_params_buffer_;
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
    Timer::Step(Physics::step_ticks_);

    for (auto const& key_group_pair : object_groups_) {
      ObjectGroupContainer* group_container = key_group_pair.second;
      int i = -1;
      for (auto& object_container : group_container->objects) {
        Object* object = object_container.object;
        object->Step();
        Point coord = object_container.v.Begin();
        for (auto const& shape_coords_params :
             *object_container.object->CoordsParams()) {
          group_container->coords_params_buffer_.at(++i) =
              coord.x + shape_coords_params.at(0);
          group_container->coords_params_buffer_.at(++i) =
              coord.y + shape_coords_params.at(1);
          group_container->coords_params_buffer_.at(++i) =
              coord.z + shape_coords_params.at(2);
          group_container->coords_params_buffer_.at(++i) =
              shape_coords_params.at(3);
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
  static int step_ticks_;
};

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
    std::vector<float> coords_params_buffer_;
    std::size_t sizeof_coords_params_buffer_;
  };
  std::unordered_map<int, ObjectGroupContainer*> object_groups_;

 public:
  void SetupObject(int key, Object* reference_object, ObjectsStaticInfo& osi) {
    assert(object_groups_.find(key) == object_groups_.end());
    auto container = new ObjectGroupContainer;
    container->reference_object_ = reference_object;
    container->num_shapes = 0;
    object_groups_[key] = container;
  }

  void AddObject(int key, Object* object) {
    ObjectGroupContainer* group = FindGroupContainer(key);
    group->objects.push_back(object);
    const int current_size = group->coords_params_buffer_.size();
    const int shapes_in_object =
        group->reference_object_->CoordsParams().size();
    assert(shapes_in_object > 0);
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
    Timer::Step(Physics::frame_ms_);

    for (auto const& key_group_pair : object_groups_) {
      ObjectGroupContainer* group_container = key_group_pair.second;
      int i = -1;
      for (auto const& object : group_container->objects) {
        object->Step();
        Object* owner = object->Owner();

        Point coord;
        if (owner)
          coord = owner->V().Begin() + owner->AttachmentPoint();
        else {
          const Vec g{0, 0, 0, 0, -0.02, 0};  // ускор свобод падения
          if (object->Weight() > 0) {  // динамический объект
            object->V() = object->V() + g;
            object->V() = object->V() >> 0.1;
          }
          coord = object->V().Begin();
        }
        for (auto const& shape_coords_params : object->CoordsParams()) {
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
  static int frame_ms_;
  static float frame_fraction_sec_;
};

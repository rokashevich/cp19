#pragma once

#include <cassert>
#include <iostream>
#include <list>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "object.hpp"
#include "timer.hpp"

class Physics : protected Timer {
  // Структура ставится в соответствие каждому типу объекта, и хранит сами
  // объекты, а так же некоторые нужные "физические" величины эти объектов.
  struct ObjectGroupContainer {
    bool is_dynamic;
    Object* reference_object_;
    std::vector<Object*> objects;
    std::vector<float> coords_params_buffer_;
    std::size_t sizeof_coords_params_buffer_;
  };
  std::unordered_map<std::type_index, ObjectGroupContainer*> group_;

 public:
  void SetupObject(std::type_index key, Object* reference_object,
                   bool is_dynamic) {
    assert(group_.find(key) == group_.end());
    auto container = new ObjectGroupContainer;
    container->is_dynamic = is_dynamic;
    container->reference_object_ = reference_object;
    group_[key] = container;
  }

  void AddObject(std::type_index key, Object* object) {
    auto it = group_.find(key);
    assert(it != group_.end());
    ObjectGroupContainer* contaier = group_.find(key)->second;
    contaier->objects.push_back(object);
    contaier->coords_params_buffer_.push_back(object->x());
    contaier->coords_params_buffer_.push_back(object->y());
    contaier->coords_params_buffer_.push_back(object->z());
    contaier->coords_params_buffer_.push_back(object->w());
    contaier->sizeof_coords_params_buffer_ =
        sizeof(float) * contaier->coords_params_buffer_.size();
  }

  float* CoordsParamsBuffer(std::type_index key) {
    auto it = group_.find(key);
    assert(it != group_.end());
    ObjectGroupContainer* contaier = group_.find(key)->second;
    return contaier->coords_params_buffer_.data();
  }

  std::size_t SizeofCoordsParamsBuffer(std::type_index key) {
    auto it = group_.find(key);
    assert(it != group_.end());
    const ObjectGroupContainer* contaier = group_.find(key)->second;
    return contaier->sizeof_coords_params_buffer_;
  }

  int ObjectsCount(std::type_index key) {
    auto it = group_.find(key);
    assert(it != group_.end());
    const ObjectGroupContainer* contaier = group_.find(key)->second;
    return contaier->objects.size();
  }

  const float* ShapeVerticesBuffer(std::type_index key) {
    ObjectGroupContainer* container = FindGroupContainer(key);
    return container->reference_object_->VerticesBuffer();
  }

  void Step();

 private:
  ObjectGroupContainer* FindGroupContainer(std::type_index key) {
    auto it = group_.find(key);
    assert(it != group_.end());
    return group_.find(key)->second;
  }
  static int target_fps_;
  static int step_ticks_;
};

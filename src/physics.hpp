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
  std::function<void(int, int, const float*, int, const float*, int,
                     const float*, int)>
      ObjectsChangedCallback;

 public:
  Physics(std::function<void(int, int, const float*, int, const float*, int,
                             const float*, int)>
              objects_changed_callback) {
    ObjectsChangedCallback = objects_changed_callback;
  }
  void RegisterObject(int key) { groups_[key] = new Group; }

  // Регистрирует объект типа key.
  void AddObject(int key, Object* object);

  void Step();

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

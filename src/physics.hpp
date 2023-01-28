#pragma once

#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <list>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "constants.hpp"
#include "group.hpp"
#include "object.hpp"
#include "timer.hpp"
class Physics : protected Timer {
  // С FPS надо будет разбираться когда будет сетевая часть, чтобы и на слабом
  // телефоне, где кадр считается долго, и на новом телефоне, где кадр считается
  // быстро - скорость игры (полёт пули) были одинаковыми!
  static constexpr int target_fps_{20};
  static constexpr int frame_ms_{1000 / target_fps_};
  static constexpr float frame_fraction_sec_{1.0f / Physics::target_fps_};

  std::vector<Group>* groups_;

 public:
  Physics(std::vector<Group>* groups) : groups_{groups} {}
  void Step() {
    for (auto& group : *groups_) {
      // Проходим по объектам, которые могут перемещаться: игрокам, пулям,
      // платформам. И перемещаем их (например, игроку пользователь задал идти
      // вперёд-вправо - вот, перемещаем его вперёд-вправо).
      // TODO Надо добавить логику запоминани предыдущей позиции, т.к.
      // столкновение могло произойти в середине отрезка пермещения.
      if (!group.stationary) {
        for (auto& object : group.objects) {
          object->coords.x -= object->left_right;
          object->coords.y += object->up_down;
          object->coords.z += object->backward_forward;
          object->backward_forward = 0;
          object->left_right = 0;
          object->up_down = 0;
        }
      }

      // Применяем силу притяжения.
      if (!group.stationary) {
        for (auto& object : group.objects) {
          object->coords.y -= 0.1;
        }
      }

      // Для каждой группы генерируем одномерный массив floatoв с координатами,
      // углами и параметрами для рендерара для передачи в шейдер. Делаем это в
      // физике, а не в рендерере, т.к. здесь это будет быстрее.
      group.coords_buffer.clear();
      group.angles_buffer.clear();
      group.params_buffer.clear();
      for (auto& object : group.objects) {
        group.coords_buffer.push_back(object->coords.x);
        group.coords_buffer.push_back(object->coords.y);
        group.coords_buffer.push_back(object->coords.z);

        group.angles_buffer.push_back(object->angles.x);
        group.angles_buffer.push_back(object->angles.y);
        group.angles_buffer.push_back(object->angles.z);

        group.params_buffer.push_back(object->params.x);
        group.params_buffer.push_back(object->params.y);
        group.params_buffer.push_back(object->params.z);
        group.params_buffer.push_back(object->params.w);
      }
    }

    // Досыпаем до целевого FPS.
    Timer::SleepAlign(Physics::frame_ms_);
  }
};

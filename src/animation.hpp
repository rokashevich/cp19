#pragma once
// Назначение:
// Получая в конструкторе размеры головы, туловища, тела, рук и ног класс
// рассчитывает координаты скелетных анимаций для разных действий (идти,
// подкрыгивать, разворачиваться и т.д.). Объект игрока содержит этот созданный
// экземпляр анимации.

#include "glm/glm.hpp"

class Animation {
 public:
  enum class Sequence { kWalk };
  struct CurrentCoords {};
  Animation(int head, int body, int arms, int legs);
  void StartSequence(Sequence sequence);
  int Next() {
    static size_t step{0};
    std::vector<int> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const auto k{5.0f / a[step]};
    ++step;
    if (step == a.size()) step = 0;
    return k;
  }
};
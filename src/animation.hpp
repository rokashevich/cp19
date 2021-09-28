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
  int Next();
};
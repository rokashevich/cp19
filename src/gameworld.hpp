#ifndef GAMEWORLD_HPP
#define GAMEWORLD_HPP

#include <array>
#include <iostream>
#include <vector>

// Состояние игрового мира.

struct Panel {
  int health;
  int material;
};
class GameWorld {
 public:
  GameWorld();
  ~GameWorld();
  float* InstancedArray() { return instanced_array_.data(); }
  int InstancedArraySize() { return instanced_array_.size(); }

 private:
  int resolution_;  // Размерность куба игрового мира.
  int surfaces_count_;

  // Стены описываются трёмя массивами плоскостей: стены в плоскостях xy, yz и
  // перекрытия (полы, потолки) в плоскости xz. Каждых наборов плоскостей по
  // dimension_ + 1 т.е. на одну больше размерности игрового куба.
  std::array<std::vector<std::vector<std::vector<Panel> > >, 3> skeleton_;

  std::vector<float> instanced_array_;
};

#endif  // GAMEWORLD_HPP

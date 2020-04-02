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
  GameWorld(int resolution);
  ~GameWorld();
  enum { kXYZ = 3, kPanelsPermanentParamsCount = 7 };

  int* panels_permanent_parameters() {
    return panels_permanent_parameters_.data();
  }
  float* panels_instanced_array() { return panels_instanced_.data(); }
  int panels_instanced_size() { return panels_instanced_.size(); }
  float* ribs_instanced_array() { return ribs_instanced_.data(); }
  int ribs_instanced_size() { return ribs_instanced_.size(); }

 private:
  std::array<int, kPanelsPermanentParamsCount> panels_permanent_parameters_;

  // Стены описываются трёмя массивами плоскостей: стены в плоскостях xy, yz и
  // перекрытия (полы, потолки) в плоскости xz. Каждых наборов плоскостей по
  // resolution_ + 1 т.е. на одну больше размерности игрового куба.
  std::vector<float> panels_instanced_;

  // Подготовленный массив рёбер.
  std::vector<float> ribs_instanced_;

  friend std::ostream& operator<<(std::ostream& os, const GameWorld& gw);
};

#endif  // GAMEWORLD_HPP

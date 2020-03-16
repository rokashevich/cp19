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

 private:
  int resolution_;  // Размерность куба игрового мира.
  int surfaces_count_;
  int slab_panels_count_;  // Количество панелей в перекрытии: поле/потолке.
  int storey_coherent_walls_count_;  // Однонаправленных стен на этаже.
  int storey_walls_count_;   // Всего стен на этаже.
  int storey_panels_total_;  // Всего панелей на этаже.
  int total_panels_;         // Всего панелей в уровне.
  std::array<int, kPanelsPermanentParamsCount> panels_permanent_parameters_;

  // Стены описываются трёмя массивами плоскостей: стены в плоскостях xy, yz и
  // перекрытия (полы, потолки) в плоскости xz. Каждых наборов плоскостей по
  // resolution_ + 1 т.е. на одну больше размерности игрового куба.
  std::array<std::vector<std::vector<std::vector<Panel> > >, kXYZ> skeleton_;

  std::vector<float> panels_instanced_;

  friend std::ostream& operator<<(std::ostream& os, const GameWorld& gw);
};

#endif  // GAMEWORLD_HPP

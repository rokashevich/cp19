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
  enum {
    kSurfaceXY = 0,
    kSurfaceYZ = 1,
    kSurfaceXZ = 2,
    kSurfacesCount = 3,
    kPanelsPermanentParamsCount = 7,
    kPanelDataSize = 4
  };

  int* panels_permanent_parameters() {
    return panels_permanent_parameters_.data();
  }

  float* panels_instanced_array() { return panels_instanced_.data(); }
  int panels_instanced_size() { return panels_instanced_.size(); }
  std::vector<float>& panels_data_array() { return panels_data_; }
  int panels_count() { return panels_count_; }

 private:
  const float panel_size_;
  std::array<int, kPanelsPermanentParamsCount> panels_permanent_parameters_;

  // Стены описываются трёмя массивами плоскостей: стены в плоскостях xy, yz и
  // перекрытия (полы, потолки) в плоскости xz. Каждых наборов плоскостей по
  // resolution_ + 1 т.е. на одну больше размерности игрового куба.
  std::vector<float> panels_instanced_;
  std::vector<float> panels_data_;
  int panels_count_;
  std::array<std::vector<std::vector<std::vector<float>>>, kSurfacesCount>
      panels_;

  friend std::ostream& operator<<(std::ostream& os, const GameWorld& gw);
};

#endif  // GAMEWORLD_HPP

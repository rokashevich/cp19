#include "gameworld.hpp"

#include <array>
#include <iostream>

#include "helpers/maze2d.hpp"

GameWorld::GameWorld(int resolution) {
  const int surfaces_count = resolution + 1;
  const int slab_panels_count = resolution * resolution;
  const int storey_coherent_walls_count = resolution * surfaces_count;
  const int storey_walls_count = storey_coherent_walls_count * 2;
  const int storey_panels_total = storey_walls_count + slab_panels_count;
  const int total_panels = storey_panels_total * resolution + slab_panels_count;

  panels_permanent_parameters_[0] = resolution;
  panels_permanent_parameters_[1] = surfaces_count;
  panels_permanent_parameters_[2] = slab_panels_count;
  panels_permanent_parameters_[3] = storey_coherent_walls_count;
  panels_permanent_parameters_[4] = storey_walls_count;
  panels_permanent_parameters_[5] = storey_panels_total;
  panels_permanent_parameters_[6] = total_panels;

  // Генерируем лабиринт.

  std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>>
      maze = Helpers::GenerateMaze(resolution);

  for (int i = 0; i < total_panels; ++i) {
    const int storey = i / storey_panels_total;
    int range = i - storey * storey_panels_total;
    if (range < slab_panels_count) {  // Пол.
      panels_instanced_.push_back(100001);
    } else if (range >= slab_panels_count + storey_coherent_walls_count) {
      std::vector<std::vector<bool>>& maze_yz = maze.first;
      range = range - slab_panels_count - storey_coherent_walls_count;
      const int col = range / resolution;
      const int row = range - col * resolution;
      const bool wall = maze_yz.at(col).at(row);
      if (wall)
        panels_instanced_.push_back(100002);
      else
        panels_instanced_.push_back(100001);
    } else {
      std::vector<std::vector<bool>>& maze_xy = maze.second;
      range = range - slab_panels_count;
      const int row = range / resolution;
      const int col = range - row * resolution;
      const bool wall = maze_xy.at(row).at(col);
      if (wall)
        panels_instanced_.push_back(100003);
      else
        panels_instanced_.push_back(100001);
    }
  }

  // Генерируем рабочую структуру панелей.
  for (int i = 0; i < kSurfacesCount; ++i) {
    panels_.at(i).resize(surfaces_count);
    for (int j = 0; j < surfaces_count; ++j) {
      panels_.at(i).at(j).resize(resolution);
      for (int k = 0; k < resolution; ++k) {
        //        if (i == kSurfaceY)
        //          panels_.at(i).at(j).at(k).resize(resolution, 1);
        //        else
        panels_.at(i).at(j).at(k).resize(resolution, 100);
      }
    }
  }

  // Из рабочей структуры получаем инстансированный массив.
  panels_data_.clear();
  panels_count_ = 0;
  for (int i = 0; i < kSurfacesCount; ++i) {
    for (int s = 0; s < surfaces_count; ++s) {
      for (int x = 0; x < resolution; ++x) {
        for (int y = 0; y < resolution; ++y) {
          const int health = panels_.at(i).at(s).at(x).at(y);
          if (health < 0) continue;
          //          float offset_x;
          //          float offset_y;
          //          float offset_z;
          //          switch (i) {
          //            case kSurfaceX:
          //              break;
          //            case kSurfaceY:
          //              break;
          //            case kSurfaceZ:

          //              break;
          //          }
          const float offset_x = (i == kSurfaceX ? -1 : 1) * (s + 1);
          const float offset_y = (i == kSurfaceY ? -1 : 1) * (x + 1);
          const float offset_z = (i == kSurfaceZ ? -1 : 1) * (y + 1);
          std::cout << offset_x << " " << offset_y << " " << offset_z
                    << std::endl;
          panels_data_.push_back(offset_x);
          panels_data_.push_back(offset_y);
          panels_data_.push_back(offset_z);
          panels_data_.push_back(panels_count_ / 100);
          ++panels_count_;
        }
      }
    }
  }

  panels_data_.clear();
  panels_count_ = 0;

  panels_data_.push_back(0);
  panels_data_.push_back(0);
  panels_data_.push_back(0);
  panels_data_.push_back(7);
  panels_count_++;

  panels_data_.push_back(0);
  panels_data_.push_back(0);
  panels_data_.push_back(-1);
  panels_data_.push_back(1);
  panels_count_++;

  panels_data_.push_back(0.5);
  panels_data_.push_back(0);
  panels_data_.push_back(0);
  panels_data_.push_back(109);
  panels_count_++;

  panels_data_.push_back(0.5);
  panels_data_.push_back(0);
  panels_data_.push_back(0);
  panels_data_.push_back(209);
  panels_count_++;
}

GameWorld::~GameWorld(){}

std::ostream& operator<<(std::ostream& os, const GameWorld& gw) {
  os << "resolution = " << gw.panels_permanent_parameters_[0] << std::endl;
  os << "surfaces_count = " << gw.panels_permanent_parameters_[1] << std::endl;
  os << "slab_panels_count = " << gw.panels_permanent_parameters_[2]
     << std::endl;
  os << "storey_coherent_walls_count_ = " << gw.panels_permanent_parameters_[3]
     << std::endl;
  os << "storey_walls_count_ = " << gw.panels_permanent_parameters_[4]
     << std::endl;
  os << "storey_panels_total_ = " << gw.panels_permanent_parameters_[5]
     << std::endl;
  os << "total_panels_ = " << gw.panels_permanent_parameters_[6] << std::endl;
  os << "panels_instanced_.size = " << gw.panels_instanced_.size() << std::endl;
  return os;
}

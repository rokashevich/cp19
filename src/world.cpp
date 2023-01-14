#include "world.hpp"

#include <array>
#include <iostream>

#include "generator_maze2d.hpp"

World::World(const int resolution) : panel_size_{3} {
  const int surfaces_count = resolution + 1;
  const int floor_panels_count = resolution * resolution;
  const int storey_coherent_walls_count = resolution * surfaces_count;
  const int storey_walls_count = storey_coherent_walls_count * 2;
  const int storey_panels_total = storey_walls_count + floor_panels_count;
  const int total_panels =
      storey_panels_total * resolution + floor_panels_count;

  panels_permanent_parameters_[0] = resolution;
  panels_permanent_parameters_[1] = surfaces_count;
  panels_permanent_parameters_[2] = floor_panels_count;
  panels_permanent_parameters_[3] = storey_coherent_walls_count;
  panels_permanent_parameters_[4] = storey_walls_count;
  panels_permanent_parameters_[5] = storey_panels_total;
  panels_permanent_parameters_[6] = total_panels;

  // Генерируем лабиринт.

  for (int i = 0; i < kSurfacesCount; ++i) {
    panels_.at(i).resize(surfaces_count);
    for (int s = 0; s < surfaces_count; ++s) {
      panels_.at(i).at(s).resize(resolution);
      for (int x = 0; x < resolution; ++x) {
        for (int y = 0; y < resolution; ++y) {
          panels_.at(i).at(s).at(y).push_back(10);
        }
      }
    }
  }

  for (int x = 0; x < resolution; ++x) {
    std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>>
        maze = Helpers::GenerateMaze(resolution);
    for (int y = 0; y < resolution; ++y) {
      for (int i = 0; i < kSurfacesCount; ++i) {
        for (int s = 0; s < surfaces_count; ++s) {
          // Удаляем наружные стены и самый верзний потолок!
          if (((i == kSurfaceXY || i == kSurfaceYZ) &&
               (s == 0 || s == resolution)) ||
              (i == kSurfaceXZ && s == resolution))
            panels_.at(i).at(s).at(x).at(y) = -1;
          else {
            switch (i) {
              case kSurfaceXY:
                if (maze.first.at(s).at(y))
                  panels_.at(i).at(s).at(y).at(x) = 10;
                else
                  panels_.at(i).at(s).at(y).at(x) = -1;
                break;
              case kSurfaceYZ:
                if (maze.second.at(s).at(y))
                  panels_.at(i).at(s).at(y).at(x) = 8;
                else
                  panels_.at(i).at(s).at(y).at(x) = -1;
                break;
              case kSurfaceXZ:
                panels_.at(i).at(s).at(y).at(x) = 1;
            }
          }
        }
      }
    }
  }
  panels_.at(2).at(0).at(0).at(0) = 9;
  panels_.at(2).at(4).at(4).at(4) = 9;
  // Делаем проходы в полах, кроме первого этажа.
  for (int i = 1; i < resolution; ++i) {
    panels_.at(kSurfaceXZ)
        .at(i)
        .at(rand() % resolution)
        .at(rand() % resolution) = -1;
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
          const float param = i * 100 + health;  // todo del
          const float half = panel_size_ / 2;
          switch (i) {
            case kSurfaceXY:  // по умолчанию - фронтальная стена (blue)
              panels_data_.push_back(panel_size_ * x);
              panels_data_.push_back(panel_size_ * y);
              panels_data_.push_back(panel_size_ * s - half);
              panels_data_.push_back(0);
              panels_data_.push_back(0);
              panels_data_.push_back(0);
              panels_data_.push_back(0);
              panels_data_.push_back(0);
              panels_data_.push_back(0);
              break;
            case kSurfaceYZ:  // стена ребром (red)
              panels_data_.push_back(panel_size_ * s - half);
              panels_data_.push_back(panel_size_ * y);
              panels_data_.push_back(panel_size_ * x);
              panels_data_.push_back(0);
              panels_data_.push_back(90);  // вокруг y на 90
              panels_data_.push_back(0);
              panels_data_.push_back(0);
              panels_data_.push_back(0);
              panels_data_.push_back(0);

              break;
            case kSurfaceXZ:  // пол-потолок (green)
              panels_data_.push_back(panel_size_ * x);
              panels_data_.push_back(panel_size_ * s - half);
              panels_data_.push_back(panel_size_ * y);
              panels_data_.push_back(90);  // вокруг x на 90
              panels_data_.push_back(0);
              panels_data_.push_back(0);
              panels_data_.push_back(0);
              panels_data_.push_back(0);
              panels_data_.push_back(0);
              break;
          }
          ++panels_count_;
        }
      }
    }
  }
  std::cout << "Wall panels count: " << panels_count_ << std::endl;
}

World::~World() {}

std::ostream& operator<<(std::ostream& os, const World& gw) {
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

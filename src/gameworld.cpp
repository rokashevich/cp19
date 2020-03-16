#include "gameworld.hpp"

#include <array>

#include "helpers/maze2d.hpp"

GameWorld::GameWorld(int resolution) : resolution_(resolution) {
  surfaces_count_ = resolution_ + 1;
  slab_panels_count_ = resolution_ * resolution_;
  storey_coherent_walls_count_ = resolution_ * surfaces_count_;
  storey_walls_count_ = storey_coherent_walls_count_ * 2;
  storey_panels_total_ = storey_walls_count_ + slab_panels_count_;
  total_panels_ = storey_panels_total_ * resolution_ + slab_panels_count_;
  panels_permanent_parameters_[0] = resolution_;
  panels_permanent_parameters_[1] = surfaces_count_;
  panels_permanent_parameters_[2] = slab_panels_count_;
  panels_permanent_parameters_[3] = storey_coherent_walls_count_;
  panels_permanent_parameters_[4] = storey_walls_count_;
  panels_permanent_parameters_[5] = storey_panels_total_;
  panels_permanent_parameters_[6] = total_panels_;

  // Генерируем кубический лабиринт.
  const Panel initial_panel1{100, 1};
  const Panel initial_panel2{100, 2};
  for (int i = 0; i < 3; ++i) {  // Проходим по трём пластинам.
    skeleton_.at(i).resize(surfaces_count_);
    for (int j = 0; j < surfaces_count_; ++j) {
      skeleton_.at(i).at(j).resize(resolution_);
      for (int k = 0; k < resolution_; ++k) {
        const Panel& initial_panel = k % 2 ? initial_panel1 : initial_panel2;
        skeleton_.at(i).at(j).at(k).resize(resolution_, initial_panel);
      }
    }
  }

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < surfaces_count_; ++j) {
      for (int k = 0; k < resolution_; ++k) {
        for (int m = 0; m < resolution_; ++m) {
          panels_instanced_.push_back(panels_instanced_.size() % 2 ? 100000
                                                                   : 100001);
        }
      }
    }
  }

  // std::vector<std::vector<char> > maze2d = Helpers::GenerateMaze(dimension_);
  Helpers::GenerateMaze(resolution_);

  //  for (auto i : maze2d) {
  //    for (auto j : i) std::cout << j;
  //    std::cout << std::endl;
  //  }
}

GameWorld::~GameWorld(){}

std::ostream& operator<<(std::ostream& os, const GameWorld& gw) {
  os << "resolution = " << gw.resolution_ << std::endl;
  os << "surfaces_count = " << gw.surfaces_count_ << std::endl;
  os << "slab_panels_count = " << gw.slab_panels_count_ << std::endl;
  os << "storey_coherent_walls_count_ = " << gw.storey_coherent_walls_count_
     << std::endl;
  os << "storey_walls_count_ = " << gw.storey_walls_count_ << std::endl;
  os << "storey_panels_total_ = " << gw.storey_panels_total_ << std::endl;
  os << "total_panels_ = " << gw.total_panels_ << std::endl;
  os << "panels_instanced_.size = " << gw.panels_instanced_.size() << std::endl;
  return os;
}

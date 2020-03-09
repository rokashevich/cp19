#include "gameworld.hpp"

#include <array>

#include "helpers/maze2d.hpp"

GameWorld::GameWorld() {
  resolution_ = 5;  // TODO: Вынести в конфиг.
  surfaces_count_ = resolution_ + 1;

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
          instanced_array_.push_back(instanced_array_.size() % 2 ? 100000
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

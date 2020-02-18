#include "gameworld.hpp"

#include <vector>

#include "helpers/maze2d.hpp"

GameWorld::GameWorld() {
  dimension_ = 10;  // TODO: Вынести в конфиг.
  // std::vector<std::vector<char> > maze2d = Helpers::GenerateMaze(dimension_);
  Helpers::GenerateMaze(dimension_);

  //  for (auto i : maze2d) {
  //    for (auto j : i) std::cout << j;
  //    std::cout << std::endl;
  //  }
}

GameWorld::~GameWorld(){}

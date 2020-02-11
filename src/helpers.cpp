#include "helpers.hpp"
#include <vector>

namespace Helpers {

void GenerateMaze(int dimenstion) {
  dimenstion = dimenstion * 2 + 1;
  std::cout << "Start generate maze" << std::endl;
  bool maze[dimenstion][dimenstion];
  for (int i = 0; i < dimenstion; ++i) {
    for (int j = 0; j < dimenstion; ++j) {
      if ((i % 2 != 0 && j % 2 != 0) &&
          (i < dimenstion - 1 && j < dimenstion - 1))
        // Если ячейка нечетная по x и y, и при этом
        // находится в пределах стен лабиринта
        maze[i][j] = true;  //то это КЛЕТКА
      else
        maze[i][j] = false;  //в остальных случаях это СТЕНА.
    }
  }
  for (int i = 0; i < dimenstion; ++i) {
    for (int j = 0; j < dimenstion; ++j) {
      std::cout << maze[i][j];
    }
    std::cout << std::endl;
  }
}

}  // namespace Helpers

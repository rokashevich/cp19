#include <array>
#include <chrono>
#include <random>
#include <stack>
#include <thread>
#include <utility>

#include "maze2d.hpp"

unsigned int unvisitedCount(const std::vector<std::vector<char> >& maze) {
  unsigned int count = 0;
  size_t dimension = maze.size();
  for (size_t i = 0; i < dimension; i++)
    for (size_t j = 0; j < dimension; j++)
      if (maze[i][j] != '#' && maze[i][j] != ' ') count++;
  return count;
}

std::vector<std::pair<size_t, size_t>> GetNeighbours(
    std::pair<size_t, size_t> cell,
    const std::vector<std::vector<char>>& maze) {
  std::vector<std::pair<size_t, size_t>> neighbours;

  size_t dimension = maze.size();
  size_t distance = 2;
  size_t x = cell.first;
  size_t y = cell.second;
  std::pair<size_t, size_t> up = std::make_pair(x, y - distance);
  std::pair<size_t, size_t> rt = std::make_pair(x + distance, y);
  std::pair<size_t, size_t> dn = std::make_pair(x, y + distance);
  std::pair<size_t, size_t> lt = std::make_pair(x - distance, y);
  std::array<std::pair<size_t, size_t>, 4> neighbours_possible = {up, rt, dn,
                                                                  lt};
  for (size_t i = 0; i < 4; i++) {  // Для каждого направления.
    if (neighbours_possible[i].first > 0 &&
        neighbours_possible[i].first < dimension &&
        neighbours_possible[i].second > 0 &&
        neighbours_possible[i].second < dimension) {
      // Если не выходит за границы лабиринта.
      char cell_neighbour_type = maze.at(neighbours_possible[i].first)
                                     .at(neighbours_possible[i].second);
      if (cell_neighbour_type != '#' && cell_neighbour_type != ' ')
        neighbours.push_back(neighbours_possible[i]);
    }
  }
  return neighbours;
}

void RemoveWall(std::pair<int, int> first, std::pair<int, int> second,
                std::vector<std::vector<char> >& maze) {
  int xDiff = second.first - first.first;
  int yDiff = second.second - first.second;

  int addX, addY;
  addX = (xDiff != 0) ? (xDiff / abs(xDiff)) : 0;
  addY = (yDiff != 0) ? (yDiff / abs(yDiff)) : 0;

  std::pair<int, int> target;
  target.first = first.first + addX;  //координаты стенки
  target.second = first.second + addY;

  maze[static_cast<size_t>(target.second)][static_cast<size_t>(target.first)] =
      ' ';
}
namespace Helpers {

std::vector<std::vector<char>> GenerateMaze(size_t dimenstion) {
  // '#' - стена
  // ' ' - посещённая клетка
  // '?' - непосещённая клетка

  dimenstion = dimenstion * 2 + 1;

  std::vector<std::vector<char>> maze(dimenstion,
                                      std::vector<char>(dimenstion, '#'));
  for (size_t i = 0; i < dimenstion; ++i) {
    for (size_t j = 0; j < dimenstion; ++j) {
      if ((i % 2 != 0 && j % 2 != 0) &&
          (i < dimenstion - 1 && j < dimenstion - 1)) {
        maze[i][j] = '?';
      }
    }
  }

  // Инициализируем генератор случайных чисел.
  std::default_random_engine generator;

  std::pair<size_t, size_t> start_cell = std::make_pair(1, 1);
  std::pair<size_t, size_t> current_cell = start_cell;
  maze[1][1] = ' ';  // Начинаем обход с левой верхней клетки.
  std::stack<std::pair<size_t, size_t>> stack;
  do {
    std::vector<std::pair<size_t, size_t>> neighbours = GetNeighbours(
        std::make_pair(current_cell.first, current_cell.second), maze);
    if (neighbours.size() != 0) {  // У клетки есть непосещённые соседи.

      std::uniform_int_distribution<size_t> distribution(0,
                                                         neighbours.size() - 1);
      size_t rand = distribution(generator);
      std::pair<size_t, size_t> neighbour_cell =
          neighbours[rand];  // Выбираем случайного соседа.
      maze[neighbour_cell.first][neighbour_cell.second] = ' ';
      RemoveWall(current_cell, neighbour_cell, maze);
      stack.push(current_cell);
      current_cell = neighbour_cell;
      // std::this_thread::sleep_for(std::chrono::seconds(1));
    } else if (stack.size() > 0) {
      stack.pop();
      current_cell = stack.top();
    } else {  // Если нет соседей и точек в стеке, но не все точки посещены,
              // выбираем случайную из непосещенных.
    }
  } while (unvisitedCount(maze));

  return maze;
}
}  // namespace Helpers
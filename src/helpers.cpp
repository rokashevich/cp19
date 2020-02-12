#include "helpers.hpp"
#include <array>
#include <chrono>
#include <random>
#include <thread>
#include <utility>
#include <vector>

unsigned int unvisitedCount(const std::vector<std::vector<char> >& maze) {
  unsigned int count = 0;
  size_t dimension = maze.size();
  for (size_t i = 0; i < dimension; i++)
    for (size_t j = 0; j < dimension; j++)
      if (maze[i][j] != '#' && maze[i][j] != ' ') count++;
  return count;
}

std::vector<std::pair<size_t, size_t> > GetNeighbours(
    std::pair<size_t, size_t> cell, const std::vector<std::vector<char> >& maze,
    size_t dimension) {
  std::vector<std::pair<size_t, size_t> > neighbours;

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

void GenerateMaze(size_t dimenstion) {
  // '#' - стена
  // ' ' - посещённая клетка
  // '?' - непосещённая клетка

  dimenstion = dimenstion * 2 + 1;

  std::vector<std::vector<char> > maze(dimenstion,
                                       std::vector<char>(dimenstion, '#'));
  for (size_t i = 0; i < dimenstion; ++i) {
    for (size_t j = 0; j < dimenstion; ++j) {
      if ((i % 2 != 0 && j % 2 != 0) &&
          (i < dimenstion - 1 && j < dimenstion - 1)) {
        maze[i][j] = '?';
      }
    }
  }

  maze[1][1] = ' ';  // Начинаем обход с левой верхней клетки.
  std::default_random_engine generator;
  std::uniform_int_distribution<size_t> distribution(0, 3);
  do {
    std::vector<std::pair<size_t, size_t> > neighbours =
        GetNeighbours(std::make_pair(1, 1), maze, dimenstion);
    if (neighbours.size() != 0) {
      // У клетки есть непосещённые соседи.
      size_t rand = distribution(generator);
      // Выбираем случайного соседа.
      std::pair<size_t, size_t> neighbour_cell = neighbours[rand];

      // std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  } while (unvisitedCount(maze));

  for (auto i : maze) {
    for (auto j : i) std::cout << j;
    std::cout << std::endl;
  }
}
}  // namespace Helpers

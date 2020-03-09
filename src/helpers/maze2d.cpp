#include <array>
#include <chrono>
#include <random>
#include <stack>
#include <thread>
#include <utility>

#include "maze2d.hpp"

typedef std::vector<std::vector<bool>> axis;

namespace {  // Вспомогательные функции, используемые в главной функции
             // генерации лабиринта GenerateMaze.
unsigned int unvisitedCount(const std::vector<std::vector<bool>>& maze) {
  unsigned int count = 0;
  int dimension = maze.size();
  for (int i = 0; i < dimension; i++)
    for (int j = 0; j < dimension; j++)
      if (!maze[i][j]) count++;
  return count;
}

std::vector<std::pair<int, int>> GetNeighbours(
    const std::pair<int, int> cell,
    const std::vector<std::vector<bool>>& maze) {
  std::vector<std::pair<int, int>> neighbours;

  int dimension = maze.size();
  int distance = 1;
  int x = cell.first;
  int y = cell.second;
  std::pair<int, int> up = std::make_pair(x, y - distance);
  std::pair<int, int> rt = std::make_pair(x + distance, y);
  std::pair<int, int> dn = std::make_pair(x, y + distance);
  std::pair<int, int> lt = std::make_pair(x - distance, y);
  std::array<std::pair<int, int>, 4> neighbours_possible = {up, rt, dn, lt};
  for (int i = 0; i < 4; i++) {  // Для каждого направления.

    if (neighbours_possible[i].first >= 0 &&
        neighbours_possible[i].first < dimension &&
        neighbours_possible[i].second >= 0 &&
        neighbours_possible[i].second < dimension) {
      // Если не выходит за границы лабиринта.
      bool cell_visited = maze.at(neighbours_possible[i].first)
                              .at(neighbours_possible[i].second);
      //      std::cout << "cel=" << cell_visited << std::endl;
      if (!cell_visited) {
        //        std::cout << neighbours_possible[i].first << " "
        //                  << neighbours_possible[i].second << std::endl;
        neighbours.push_back(neighbours_possible[i]);
      }
    }
  }
  return neighbours;
}

void RemoveWall(std::pair<int, int> first, std::pair<int, int> second,
                std::pair<axis, axis>& grid) {
  const int addx = second.first - first.first;
  const int addy = second.second - first.second;
  axis& ax = grid.first;
  axis& ay = grid.second;

  if (addx > 0)
    ax[second.first][first.second] = false;  // Вниз.
  else if (addx < 0)
    ax[first.first][first.second] = false;  // Вверх.
  else if (addy > 0)
    ay[second.second][first.first] = false;  // Вправо.
  else if (addy < 0)
    ay[first.second][first.first] = false;  // Влево.
}

void PrintMaze(std::pair<axis, axis>& grid) {
  int d2 = grid.first.size();
  int d1 = d2 - 1;
  axis& ax = grid.first;
  axis& ay = grid.second;
  for (int i = 0; i < d2; ++i) {
    for (int j = 0; j < d1; ++j) {
      std::cout << " " << (ax[i][j] ? "─" : " ");
    }
    std::cout << std::endl;
    if (i < d1) {
      for (int j = 0; j < d2; ++j) {
        std::cout << (ay[j][i] ? "│" : " ") << " ";
      }
      std::cout << std::endl;
    }
  }
}
}  // namespace

namespace Helpers {
std::pair<axis, axis> GenerateMaze(const int dimension) {
  // Инициализируем генератор случайных чисел.
  std::random_device generator;

  const int d1 = dimension;
  const int d2 = dimension + 1;
  axis ax(d2, std::vector<bool>(d1, true));  // Горизонтальные стены.
  axis ay(d2, std::vector<bool>(d1, true));  // Вертикальные стены.
  std::pair<axis, axis> grid(ax, ay);

  std::stack<std::pair<int, int>> stack;

  // Генерируем матрицу, где все клетки непосещены.
  std::vector<std::vector<bool>> maze(d1, std::vector<bool>(d1, false));

  // Начинаем обход с левой верхней клетки.
  std::pair<int, int> current_cell(0, 0);
  maze[current_cell.first][current_cell.second] = true;
  do {
    std::vector<std::pair<int, int>> neighbours = GetNeighbours(
        std::make_pair(current_cell.first, current_cell.second), maze);
    if (neighbours.size() != 0) {
      std::uniform_int_distribution<size_t> distribution(0,
                                                         neighbours.size() - 1);
      int rand = distribution(generator);
      std::pair<int, int> neighbour_cell =
          neighbours[rand];  // Выбираем случайного соседа.
                             //      std::cout << "rand:" << rand << std::endl;
      RemoveWall(current_cell, neighbour_cell, grid);
      maze[neighbour_cell.first][neighbour_cell.second] = true;
      stack.push(current_cell);
      current_cell = neighbour_cell;
    } else if (stack.size() > 0) {
      stack.pop();
      current_cell = stack.top();
    } else {
      // Если нет соседей и точек в стеке, но не все точки посещены, выбираем
      // случайную из непосещенных.
    }
  } while (unvisitedCount(maze));

  // Отрисовка каркаса ascii символами.
  PrintMaze(grid);

  return grid;
}
}  // namespace Helpers

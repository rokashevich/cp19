#include "shapegenerator.hpp"

std::vector<float> ShapeGenerator::Cuboid(float width, float height,
                                          float depth) {
  // Фигуру строим относительно центра 0,0,0 - соотв-но точки будут удалены на
  // половины расстояний.
  const float w = width /= 2;
  const float h = height /= 2;
  const float d = depth /= 2;

  // Кол-во вершин 6 граней каждая из 2х треугольников каждый из 3х вершин -
  // итого 36 трёхмерных точек.
  // TODO: В будущем в кач-ве оптимизации исключить повторяющиеся точки,
  // но тогда надо будет и код рендера переписать.
  return std::vector<float>{
      // Нижняя крышка.
      -w, -h, -d, -w, -h, d, w, -h, -d, w, -h, d, -w, -h, d, w, -h, -d,
      // Верхняя крышка.
      -w, h, -d, -w, h, d, w, h, -d, w, h, d, -w, h, d, w, h, -d,
      // Стенка xy.
      -w, -h, d, -w, h, d, w, -h, d, w, h, d, -w, h, d, w, -h, d,
      // Стенка zy.
      w, -h, -d, w, h, -d, w, -h, d, w, h, d, w, h, -d, w, -h, d,
      // Стенка -xy.
      -w, -h, -d, -w, h, -d, w, -h, -d, w, h, -d, -w, h, -d, w, -h, -d,
      // Стенка -zy.
      -w, -h, -d, -w, h, -d, -w, -h, d, -w, h, d, -w, h, -d, -w, -h, d};
}

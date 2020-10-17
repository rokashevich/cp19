#include "shapegenerator.hpp"

#include <cmath>

#include "misc.hpp"

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

std::vector<float> ShapeGenerator::Icosphere(float diameter) {
  // Алгоритм построения сферы из треугольников:
  // http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
  float r = diameter / 2;
  const float t = r * constants::golden_ratio;

  struct Point {
    float x, y, z;
  };

  struct Triangle {
    Point a, b, c;
  };

  const std::vector<Point> vertices{
      Point{-r, t, 0}, Point{r, t, 0}, Point{-r, -t, 0}, Point{r, -t, 0},
      Point{0, -r, t}, Point{0, r, t}, Point{0, -r, -t}, Point{0, r, -t},
      Point{t, 0, -r}, Point{t, 0, r}, Point{-t, 0, -r}, Point{-t, 0, r}};
  const std::vector<Triangle> faces{
      Triangle{vertices.at(0), vertices.at(11), vertices.at(5)},
      Triangle{vertices.at(0), vertices.at(5), vertices.at(1)},
      Triangle{vertices.at(0), vertices.at(1), vertices.at(7)},
      Triangle{vertices.at(0), vertices.at(7), vertices.at(10)},
      Triangle{vertices.at(0), vertices.at(10), vertices.at(11)},
      Triangle{vertices.at(1), vertices.at(5), vertices.at(9)},
      Triangle{vertices.at(5), vertices.at(11), vertices.at(4)},
      Triangle{vertices.at(11), vertices.at(10), vertices.at(2)},
      Triangle{vertices.at(10), vertices.at(7), vertices.at(6)},
      Triangle{vertices.at(7), vertices.at(1), vertices.at(8)},
      Triangle{vertices.at(3), vertices.at(9), vertices.at(4)},
      Triangle{vertices.at(3), vertices.at(4), vertices.at(2)},
      Triangle{vertices.at(3), vertices.at(2), vertices.at(6)},
      Triangle{vertices.at(3), vertices.at(6), vertices.at(8)},
      Triangle{vertices.at(3), vertices.at(8), vertices.at(9)},
      Triangle{vertices.at(4), vertices.at(9), vertices.at(5)},
      Triangle{vertices.at(2), vertices.at(4), vertices.at(11)},
      Triangle{vertices.at(6), vertices.at(2), vertices.at(10)},
      Triangle{vertices.at(8), vertices.at(6), vertices.at(7)},
      Triangle{vertices.at(9), vertices.at(8), vertices.at(1)}};
  std::vector<float> buf;
  for (const auto& face : faces) {
    buf.push_back(face.a.x);
    buf.push_back(face.a.y);
    buf.push_back(face.a.z);
    buf.push_back(face.b.x);
    buf.push_back(face.b.y);
    buf.push_back(face.b.z);
    buf.push_back(face.c.x);
    buf.push_back(face.c.y);
    buf.push_back(face.c.z);
  }
  return buf;
}

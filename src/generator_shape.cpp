#include "generator_shape.hpp"

#include <cmath>
#include <iostream>
#include <map>
#include <memory>

#include "constants.hpp"
#include "point.hpp"

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

std::vector<float> ShapeGenerator::Icosphere() {
  // Алгоритм построения сферы из треугольников:
  // http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html

  struct TriangleIndices {
    int v1, v2, v3;

    TriangleIndices(int v1, int v2, int v3) {
      this->v1 = v1;
      this->v2 = v2;
      this->v3 = v3;
    }
  };

  class IcoSphere {
   public:
    IcoSphere() {}

    ~IcoSphere() {}

    std::vector<float> Create(int recursionLevel) {
      middlePointIndexCache.clear();
      vertices.clear();
      indices.clear();
      index = 0;

      const auto t = constants::golden_ratio;

      AddVertex(Point(-1, t, 0));
      AddVertex(Point(1, t, 0));
      AddVertex(Point(-1, -t, 0));
      AddVertex(Point(1, -t, 0));

      AddVertex(Point(0, -1, t));
      AddVertex(Point(0, 1, t));
      AddVertex(Point(0, -1, -t));
      AddVertex(Point(0, 1, -t));

      AddVertex(Point(t, 0, -1));
      AddVertex(Point(t, 0, 1));
      AddVertex(Point(-t, 0, -1));
      AddVertex(Point(-t, 0, 1));

      auto faces = std::vector<std::shared_ptr<TriangleIndices>>();
      faces.push_back(std::make_shared<TriangleIndices>(0, 11, 5));
      faces.push_back(std::make_shared<TriangleIndices>(0, 5, 1));
      faces.push_back(std::make_shared<TriangleIndices>(0, 1, 7));
      faces.push_back(std::make_shared<TriangleIndices>(0, 7, 10));
      faces.push_back(std::make_shared<TriangleIndices>(0, 10, 11));

      faces.push_back(std::make_shared<TriangleIndices>(1, 5, 9));
      faces.push_back(std::make_shared<TriangleIndices>(5, 11, 4));
      faces.push_back(std::make_shared<TriangleIndices>(11, 10, 2));
      faces.push_back(std::make_shared<TriangleIndices>(10, 7, 6));
      faces.push_back(std::make_shared<TriangleIndices>(7, 1, 8));

      faces.push_back(std::make_shared<TriangleIndices>(3, 9, 4));
      faces.push_back(std::make_shared<TriangleIndices>(3, 4, 2));
      faces.push_back(std::make_shared<TriangleIndices>(3, 2, 6));
      faces.push_back(std::make_shared<TriangleIndices>(3, 6, 8));
      faces.push_back(std::make_shared<TriangleIndices>(3, 8, 9));

      faces.push_back(std::make_shared<TriangleIndices>(4, 9, 5));
      faces.push_back(std::make_shared<TriangleIndices>(2, 4, 11));
      faces.push_back(std::make_shared<TriangleIndices>(6, 2, 10));
      faces.push_back(std::make_shared<TriangleIndices>(8, 6, 7));
      faces.push_back(std::make_shared<TriangleIndices>(9, 8, 1));

      for (auto i = 0; i < recursionLevel; ++i) {
        auto faces2 = std::vector<std::shared_ptr<TriangleIndices>>();
        for (auto tri : faces) {
          int a = GetMiddlePoint(tri->v1, tri->v2);
          int b = GetMiddlePoint(tri->v2, tri->v3);
          int c = GetMiddlePoint(tri->v3, tri->v1);

          faces2.push_back(std::make_shared<TriangleIndices>(tri->v1, a, c));
          faces2.push_back(std::make_shared<TriangleIndices>(tri->v2, b, a));
          faces2.push_back(std::make_shared<TriangleIndices>(tri->v3, c, b));
          faces2.push_back(std::make_shared<TriangleIndices>(a, b, c));
        }

        faces.clear();
        faces.swap(faces2);
      }

      std::vector<float> buffer;
      for (auto tri : faces) {
        this->indices.push_back(tri->v1);
        this->indices.push_back(tri->v2);
        this->indices.push_back(tri->v3);

        buffer.push_back(vertices.at(tri->v1).x);
        buffer.push_back(vertices.at(tri->v1).y);
        buffer.push_back(vertices.at(tri->v1).z);

        buffer.push_back(vertices.at(tri->v2).x);
        buffer.push_back(vertices.at(tri->v2).y);
        buffer.push_back(vertices.at(tri->v2).z);

        buffer.push_back(vertices.at(tri->v3).x);
        buffer.push_back(vertices.at(tri->v3).y);
        buffer.push_back(vertices.at(tri->v3).z);
      }
      std::cout << "faces count = " << faces.size() << std::endl;
      std::cout << buffer.size() << std::endl;

      return buffer;
    }

   private:
    int AddVertex(Point position) {
      double length = sqrt(position.x * position.x + position.y * position.y +
                           position.z * position.z);
      vertices.push_back(
          Point(position.x / length, position.y / length, position.z / length));

      return index++;
    }

    int GetMiddlePoint(int p1, int p2) {
      bool firstPointIsSmaller = p1 < p2;
      int64_t smallerIndex = firstPointIsSmaller ? p1 : p2;
      int64_t greaterIndex = firstPointIsSmaller ? p2 : p1;
      int64_t key = (smallerIndex << 32) + greaterIndex;

      auto foundValueIterator = middlePointIndexCache.find(key);
      if (foundValueIterator != middlePointIndexCache.end()) {
        return foundValueIterator->second;
      }

      Point point1 = vertices[p1];
      Point point2 = vertices[p2];
      Point middle =
          Point((point1.x + point2.x) / 2.0, (point1.y + point2.y) / 2.0,
                (point1.z + point2.z) / 2.0);

      int i = this->AddVertex(middle);

      this->middlePointIndexCache.insert(std::make_pair(key, i));
      return i;
    }

   public:
    std::vector<Point> vertices;
    std::vector<unsigned int> indices;

   private:
    int index;
    std::map<int64_t, int> middlePointIndexCache;
  };
  IcoSphere i = IcoSphere();
  return i.Create(2);
}

#pragma once
#include <cstddef>
#include <vector>
// Класс, описывающий геометрическую форму объекта: массив трёхмерных точек, из
// которых этот объект состоит, кол-во этих точек, размер массива в байтах,
// кол-во треугольников, которые формируются этими точками - всё это
// технологические данные, нужные для функций рендеринга OpenGL.
template <class T>
struct Shape {
  using t = float;  // Для возможной быстрой смены float.
  static const std::vector<t> data;
  static const Shape<T>::t* Data() { return Shape<T>::data.data(); }
  static int NumBytes() { return Shape<T>::data.size() * sizeof(t); }
  static int NumVertices() { return Shape<T>::data.size() / 3; }
  static int NumTriangles() { return Shape<T>::NumVertices() / 3; }

  static void* static_setup_data;
};

#pragma once
#include <vector>

#include "object.hpp"
#include "point.hpp"
#include "vec.hpp"
// Солдат.
class ObjectPlayer : public Object {
  static const std::vector<float> vertices_buffer_;
  static const int num_vertices_;

  int legs_;
  int body_;
  int head_;
  int arms_;
  int gun_;

 public:
  ObjectPlayer(P position, P direction, int legs = 9, int body = 9,
               int head = 9, int arms = 9, int gun = 9);
  ~ObjectPlayer();
  float w() final { return 1; }
  const float* VerticesBuffer() final;
  std::size_t SizeofVerticesBuffer() final;
  int NumVertices() final;
};

#pragma once
#include <vector>

#include "helpers/shapegenerator.hpp"
#include "object.hpp"
#include "shape.hpp"

// Стена.
class ObjectWall : public Object, Shape<ObjectWall> {
  int health_;

 public:
  ObjectWall(int helath);
  ~ObjectWall();
  void RenderParameters(std::vector<float>& p) override;
};

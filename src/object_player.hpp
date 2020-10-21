#pragma once
#include <vector>

#include "object.hpp"
#include "point.hpp"
#include "shape.hpp"
#include "vec.hpp"
// Солдат.
class ObjectPlayer : public Object, Shape<ObjectPlayer> {
  int legs_;
  int body_;
  int head_;
  int arms_;
  int gun_;

 public:
  ObjectPlayer(P position, P direction, int legs = 9, int body = 9,
               int head = 9, int arms = 9, int gun = 9);
  ~ObjectPlayer();
  virtual void RenderParameters(std::vector<float>& p) override;
};

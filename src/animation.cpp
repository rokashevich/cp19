#include "animation.hpp"

#include <cstddef>
#include <vector>
Animation::Animation(int head, int body, int arms, int legs) {}
int Animation::Next() {
  static size_t step{0};
  std::vector<int> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  const auto k{5.0f / a[step]};
  ++step;
  if (step == a.size()) step = 0;
  return k;
}
void Animation::StartSequence(Sequence sequence) {}
#include <gtest/gtest.h>

#include "vec.hpp"

TEST(ClassVec, TestSummation) {
  Vec A{{0, 0, 0}, {1, 0, 0}};
  Vec B{{0, 0, 0}, {1, 0, 0}};
  Vec C = A + B;
  EXPECT_EQ(2, C.End().x);

  Vec D{{0, 0, 0}, {-3, 0, 0}};
  Vec E = C + D;
  EXPECT_EQ(-1, E.End().x);
}

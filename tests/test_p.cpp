#include <gtest/gtest.h>

#include "p.hpp"

TEST(ClassPoint, TestDistance) {
  P A{4, 0, 0};
  EXPECT_EQ(0, A.DistanceTo(A));

  P B{0, 0, 0};
  EXPECT_EQ(4, A.DistanceTo(B));
}

TEST(ClassPoint, TestSubstraction) {
  P A{2, 2, 2};
  P B{1, 2, 3};
  P C = A - B;
  EXPECT_EQ(1, C.x);
  EXPECT_EQ(0, C.y);
  EXPECT_EQ(-1, C.z);
}

TEST(ClassPoint, TestSummation) {
  P A{9, 99, -1000};
  P B{1, 1, 1};
  P C = A + B;
  EXPECT_EQ(10, C.x);
  EXPECT_EQ(100, C.y);
  EXPECT_EQ(-999, C.z);
}

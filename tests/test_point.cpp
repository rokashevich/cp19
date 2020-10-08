#include <gtest/gtest.h>

#include "point.hpp"

TEST(ClassPoint, TestDistance) {
    Point A {4,0,0};
    EXPECT_EQ(0, A.DistanceTo(A));

    Point B {0,0,0};
    EXPECT_EQ(4, A.DistanceTo(B));
}

TEST(ClassPoint, TestSubstraction) {
  Point A{2, 2, 2};
  Point B{1, 2, 3};
  Point C = A - B;
  EXPECT_EQ(1, C.x);
  EXPECT_EQ(0, C.y);
  EXPECT_EQ(-1, C.z);
}

TEST(ClassPoint, TestSummation) {
  Point A{9, 99, -1000};
  Point B{1, 1, 1};
  Point C = A + B;
  EXPECT_EQ(10, C.x);
  EXPECT_EQ(100, C.y);
  EXPECT_EQ(-999, C.z);
}

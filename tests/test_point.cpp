#include <gtest/gtest.h>

#include "point.hpp"

TEST(ClassPoint, TestDistance) {
    Point A {4,0,0};
    EXPECT_EQ(0, A.DistanceTo(A));

    Point B {0,0,0};
    EXPECT_EQ(4, A.DistanceTo(B));
}

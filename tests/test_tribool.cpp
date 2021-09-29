#include <gtest/gtest.h>

#include <limits>

#include "t.hpp"

class TriboolTest : public ::testing::Test {
 protected:
  void SetUp() override {
    p = std::numeric_limits<int>::max();
    z = 0;
    n = std::numeric_limits<int>::min();
  }
  tribool p;
  tribool z;
  tribool n;
  tribool p1{1};
  tribool p2{2};
  tribool z1{0};
  tribool z2{0};
  tribool n1{-1};
  tribool n2{-2};
};

TEST_F(TriboolTest, Comparison) {
  EXPECT_EQ(true, p != n);

  EXPECT_EQ(true, p == p1);
  EXPECT_EQ(true, n == n1);
  EXPECT_EQ(true, p1 == p2);
  EXPECT_EQ(true, z1 == z2);
  EXPECT_EQ(true, n1 == n2);

  EXPECT_EQ(true, p > z);
  EXPECT_EQ(true, p > n);
  EXPECT_EQ(true, z > n);
  EXPECT_EQ(true, p >= z);
  EXPECT_EQ(true, p >= n);
  EXPECT_EQ(true, z >= n);

  EXPECT_EQ(true, n < z);
  EXPECT_EQ(true, z < p);
  EXPECT_EQ(true, n < p);
  EXPECT_EQ(true, n <= z);
  EXPECT_EQ(true, z <= p);
  EXPECT_EQ(true, n <= p);
}

TEST_F(TriboolTest, PrefixPostfix) { EXPECT_EQ(true, --p == ++n); }

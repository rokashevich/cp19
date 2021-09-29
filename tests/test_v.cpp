#include <gtest/gtest.h>

#include "t.hpp"

TEST(V, Equation) {
  EXPECT_EQ(V(1, 2, 3, 4, 5, 6), V(1, 2, 3, 4, 5, 6));
  EXPECT_EQ(V(1, 2, 3), V(1, 2, 3));

  //  Vec A{{0, 0, 0}, {1, 0, 0}};
  //  Vec B{{0, 0, 0}, {1, 0, 0}};
  //  Vec C = A + B;
  //  EXPECT_EQ(2, C.End().x);

  //  Vec D{{0, 0, 0}, {-3, 0, 0}};
  //  Vec E = C + D;
  //  EXPECT_EQ(-1, E.End().x);

  //  Vec F{{0, 0, 0}, {27, 0, 0}};
  //  EXPECT_EQ(9, (F / 3).End().x);
  //  EXPECT_EQ(54, (F >> 2).Begin().x);
}

// https://onlinemschool.com/math/assistance/vector/calc/

TEST(V, Plus) {
  EXPECT_EQ(V(1, 1, 1) + V(1, 1, 1), V(2, 2, 2));
  EXPECT_EQ(V(1, 1, 1, 2, 2, 2) + V(2, 2, 2, 3, 3, 3), V(1, 1, 1, 3, 3, 3));
  EXPECT_EQ(V(1, 1, 1, 3, 3, 3) + V(0, -1, 0), V(1, 1, 1, 3, 2, 3));
}

TEST(V, Shift) {
  EXPECT_EQ(V(1, 1, 1, 3, 3, 3) >> 0.5, V(2, 2, 2, 4, 4, 4));
  EXPECT_EQ(V(3, 3, 3, 1, 1, 1) >> 0.5, V(2, 2, 2, 0, 0, 0));
}

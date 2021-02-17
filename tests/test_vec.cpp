#include <gtest/gtest.h>

#include "vec.hpp"

TEST(Vec, Equation) {
  EXPECT_EQ(Vec(1, 2, 3, 4, 5, 6), Vec(1, 2, 3, 4, 5, 6));
  EXPECT_EQ(Vec(1, 2, 3), Vec(1, 2, 3));

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

TEST(Vec, Plus) {
  EXPECT_EQ(Vec(1, 1, 1) + Vec(1, 1, 1), Vec(2, 2, 2));
  EXPECT_EQ(Vec(1, 1, 1, 2, 2, 2) + Vec(2, 2, 2, 3, 3, 3),
            Vec(1, 1, 1, 3, 3, 3));
  EXPECT_EQ(Vec(1, 1, 1, 3, 3, 3) + Vec(0, -1, 0), Vec(1, 1, 1, 3, 2, 3));
}

TEST(Vec, Shift) {
  EXPECT_EQ(Vec(1, 1, 1, 3, 3, 3) >> 0.5, Vec(2, 2, 2, 4, 4, 4));
  EXPECT_EQ(Vec(3, 3, 3, 1, 1, 1) >> 0.5, Vec(2, 2, 2, 0, 0, 0));
}

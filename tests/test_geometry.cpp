#include "Geometry.h"
#include <gtest/gtest.h>

TEST(LORTest, TestLORDetectorPosition) {
  auto lor = LOR::from_idxs(5, 0, 0);
  ScannerGeometry geometry({10, 10}, 1, {0, 5}, 10, 4);
  auto result = lor.get_det_positions(geometry);
  EXPECT_NEAR(result.first.x, 9.974937, 0.00001);
  EXPECT_DOUBLE_EQ(result.first.y, 0.5);
  EXPECT_NEAR(result.second.x, 0.02506281, 0.00001);
  EXPECT_DOUBLE_EQ(result.second.y, 0.5);
}

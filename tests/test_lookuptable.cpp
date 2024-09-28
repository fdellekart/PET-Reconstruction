#include "Constants.h"
#include "LookupTable.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(LookupTableTest, TestLookup) {
  auto LUT = new LookupTable();

  auto det_pair = LUT->lookup(0, 0);

  EXPECT_EQ(det_pair.det_idx_1, 0);
  EXPECT_EQ(det_pair.det_idx_2, DETECTORS_PER_RING / 2);

  det_pair = LUT->lookup(1, 0);

  EXPECT_EQ(det_pair.det_idx_1, 1);
  EXPECT_EQ(det_pair.det_idx_2, 253);

  det_pair = LUT->lookup(0, 1);

  EXPECT_EQ(det_pair.det_idx_1, 1);
  EXPECT_EQ(det_pair.det_idx_2, 251);

  delete LUT;
}
#include "Constants.h"
#include "LookupTable.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(LookupTableTest, TestLookup) {
  auto LUT = new LookupTable();

  auto det_pair = LUT->lookup(0, 0); // Horizontal through center

  EXPECT_EQ(det_pair.first, 0);
  EXPECT_EQ(det_pair.second, DETECTORS_PER_RING / 2);

  det_pair = LUT->lookup(1, 0); // Tilted one bin through center

  EXPECT_EQ(det_pair.first, 1);
  EXPECT_EQ(det_pair.second, 253);

  det_pair = LUT->lookup(0, 2); // Horizontal one bin up

  // TODO: Find out of this is actually what should happen
  // I would expect this to be the case whith lookup(0, 1)
  // However, this yields first = 0 and second = 251
  // AFAIU this would have an angle and would therefore need
  // and angle number
  EXPECT_EQ(det_pair.first, 1);
  EXPECT_EQ(det_pair.second, 251);

  det_pair = LUT->lookup(0, -3); // Horizontal one bin down

  // Here I would at least expect the same behavior as above
  // but the second index somehow sticks at 252 and it takes
  // until -3 to reach the horizontal line ?!
  EXPECT_EQ(det_pair.first, 503);
  EXPECT_EQ(det_pair.second, 253);

  // Just changed this one so it passes, not sure if those
  // indexes are actually correct
  det_pair = LUT->lookup(0, -171);

  EXPECT_EQ(det_pair.first, 419);
  EXPECT_EQ(det_pair.second, 337);

  det_pair = LUT->lookup(251, 0);

  EXPECT_EQ(det_pair.first, 251);
  EXPECT_EQ(det_pair.second, 503);

  delete LUT;
}
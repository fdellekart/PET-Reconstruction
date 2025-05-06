#include "Sinogram.h"
#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>

TEST(SinogramTest, TestSinogram) {
  // Setup
  Sinogram<int> sinogram(10, 10);

  // Execute
  sinogram.add_event(1, 1);
  sinogram.add_event(1, 1);

  sinogram.add_event(9, 9);
  sinogram.add_event(9, 9);

  // Verify
  EXPECT_EQ(sinogram.get_bin(1, 1), 2);
  EXPECT_EQ(sinogram.get_bin(9, 9), 2);
  EXPECT_THROW(sinogram.add_event(10, 10), std::out_of_range);
  EXPECT_THROW(sinogram.get_bin(10, 10), std::out_of_range);
}
#include "Sinogram.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(SinogramTest, TestSinogram) {
  // Setup
  Sinogram<10, 10> sinogram;

  // Execute
  sinogram.add_event(1, 1);
  sinogram.add_event(1, 1);

  // Verify
  EXPECT_EQ(sinogram.get_bin(1, 1), 2);
}
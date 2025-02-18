#include "RayTracer.h"
#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>

/// Test a horizontal trace from left to right
TEST(RayTracerTest, HorizontalTraceInLowestVoxel) {
  // Setup 10x10 image with voxel size one and origin 0
  RayTracer tracer({10, 10}, {1, 1}, {0, 0});
  Vec2 start, end;

  start.x = 0;
  start.y = 0.5;
  end.x = 10;
  end.y = 0.5;

  // Execute
  std::vector<VoxelHit> result = tracer.trace(start, end);

  // Verify
  int idx = 1; // TODO: Zero based could make sense
  for (auto hit : result) {
    EXPECT_EQ(hit.i, idx);
    idx++;
    if (hit.i > 10 | hit.j > 10) {
      // TODO: Some voxels outside the image are in the result
      // with zero length
      EXPECT_FLOAT_EQ(hit.length, 0);
      continue;
    }
    EXPECT_FLOAT_EQ(hit.length, 0.1);
  }
  EXPECT_EQ(std::size(result), 11); // TODO: Could be ten
}

/// Test a horizontal trace from right to left
TEST(RayTracerTest, HorizontalTraceInLowestVoxelReverse) {
  // Setup 10x10 image with voxel size one and origin 0
  RayTracer tracer({10, 10}, {1, 1}, {0, 0});
  Vec2 start, end;

  start.x = 10;
  start.y = 0.5;
  end.x = 0;
  end.y = 0.5;

  // Execute
  std::vector<VoxelHit> result = tracer.trace(start, end);

  // Verify
  int idx = 11; // TODO: Zero based could make sense
  for (auto hit : result) {
    EXPECT_EQ(hit.i, idx);
    idx--;
    if (hit.i > 10 | hit.j > 10) {
      // TODO: Some voxels outside the image are in the result
      // with zero length
      EXPECT_FLOAT_EQ(hit.length, 0);
      continue;
    }
    EXPECT_FLOAT_EQ(hit.length, 0.1);
  }
  EXPECT_EQ(std::size(result), 11); // TODO: Could be ten
}

/// Test a horizontal trace from bottom to top
TEST(RayTracerTest, VerticalTraceInLeftVoxel) {
  // Setup 10x10 image with voxel size one and origin 0
  RayTracer tracer({10, 10}, {1, 1}, {0, 0});
  Vec2 start, end;

  start.x = 0.5;
  start.y = 0;
  end.x = 0.5;
  end.y = 10;

  // Execute
  std::vector<VoxelHit> result = tracer.trace(start, end);

  // Verify
  int idx = 1; // TODO: Zero based could make sense
  for (auto hit : result) {
    EXPECT_EQ(hit.j, idx);
    idx++;
    if (hit.i > 10 | hit.j > 10) {
      // TODO: Some voxels outside the image are in the result
      // with zero length
      EXPECT_FLOAT_EQ(hit.length, 0);
      continue;
    }
    EXPECT_FLOAT_EQ(hit.length, 0.1);
  }
  EXPECT_EQ(std::size(result), 11); // TODO: Could be ten
}

/// Test a horizontal trace from top to bottom
TEST(RayTracerTest, VerticalTraceInRightVoxelReverse) {
  // Setup 10x10 image with voxel size one and origin 0
  RayTracer tracer({10, 10}, {1, 1}, {0, 0});
  Vec2 start, end;

  start.x = 9.5;
  start.y = 10;
  end.x = 9.5;
  end.y = 0;

  // Execute
  std::vector<VoxelHit> result = tracer.trace(start, end);

  // Verify
  int idx = 11; // TODO: Zero based could make sense
  for (auto hit : result) {
    EXPECT_EQ(hit.j, idx);
    idx--;
    if (hit.i > 10 | hit.j > 10) {
      // TODO: Some voxels outside the image are in the result
      // with zero length
      EXPECT_FLOAT_EQ(hit.length, 0);
      continue;
    }
    EXPECT_FLOAT_EQ(hit.length, 0.1);
  }
  EXPECT_EQ(std::size(result), 11); // TODO: Could be ten
}

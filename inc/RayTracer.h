#include "Geometry.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

struct VoxelHit {
  int i, j;      // Voxel indices
  double length; // Path length in the voxel
};

class RayTracer {
public:
  /// @param img_dimension Number of voxels for the image in each direction
  /// @param voxel_size Voxel sizes for each direction
  /// @param origin Origin of the coordinate system relative to lower left image
  /// corner
  RayTracer(Vec2<int> img_dimensions) {
    alphas.reserve(img_dimensions.x + img_dimensions.y);
  };

  /// @brief Trace the line connecting the points through the image
  /// @param ray_start Start point of line
  /// @param ray_end End point of line
  /// @return Vector of voxel hits with intersected 1-based voxel indices and
  /// intersection lengths

  /// FIXME: Currently sometimes includes voxels outside the image with
  ///        a length of 0.
  /// TODO: Adapting to 0-based indexing could be good
  std::vector<VoxelHit> trace(Vec2<double> ray_start, Vec2<double> ray_end,
                              const ScannerGeometry &geometry);

private:
  std::vector<double> alphas;
};

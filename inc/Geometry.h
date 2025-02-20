#pragma once
#include <cstdint>

template <typename T> struct Vec2 {
  T x, y;
};

struct ScannerGeometry {
  /// Number of voxels for the image in each direction
  Vec2<int> img_dimensions;

  /// Voxel sizes for each direction
  Vec2<double> voxel_size;

  /// Origin of the coordinate system relative to lower left image corner
  Vec2<double> origin;

  /// Diameter of the detector ring
  double detector_diameter;

  /// Detectors per ring
  int det_per_ring;
};

/// @brief Description of a LOR as tangential, angular and projection indexes
struct LOR {
public:
  int32_t tang_idx;
  int32_t angle_idx;
  int32_t proj_idx;
};

#pragma once
#include <cstdint>
#include <utility>

template <typename T> struct Vec2 {
  T x, y;
};

struct ScannerGeometry {
  /// Number of voxels for the image in each direction
  Vec2<int> img_dimensions;

  /// Side length of a voxel
  double voxel_size;

  /// Origin of the coordinate system relative to lower left image corner
  Vec2<double> origin;

  /// Diameter of the detector ring
  double detector_diameter;

  /// Detectors per ring
  int det_per_ring;

  /// Angular resolution
  int n_angular_positions;

  /// Tangential resolution
  int n_tangential_positions;

  static ScannerGeometry mmr() {
    return ScannerGeometry({344, 344}, 656.0 / 344, {0, 328}, 656.0, 504, 252,
                           344);
  }
};

/// @brief Description of a LOR as tangential, angular and projection indexes
struct LOR {
public:
  int32_t tang_idx;
  int32_t angle_idx;
  int32_t proj_idx;

  // Calculate the intersection points with the detector ring
  std::pair<Vec2<double>, Vec2<double>>
  get_det_positions(const ScannerGeometry &);

  inline static LOR from_idxs(int32_t tang_idx, int32_t angle_idx,
                              int32_t proj_idx) {
    return LOR(tang_idx, angle_idx, proj_idx);
  }
};

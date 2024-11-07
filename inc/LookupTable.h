#include "Constants.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <utility>

class Detector {
public:
  Detector() : tangential_idx(0), axial_idx(0) {};
  Detector(int32_t tangential_idx, int32_t axial_idx)
      : tangential_idx(tangential_idx), axial_idx(axial_idx) {};

  // Index in ring, 0 to 503
  int32_t tangential_idx;

  // Index of ring, 0 to 64
  int32_t axial_idx;
};

class DetectorPair {
public:
  DetectorPair() : first(), second() {};
  DetectorPair(Detector first, Detector second)
      : first(first), second(second) {};
  Detector first;
  Detector second;
};

/// @brief Lookuptable to transform a LOR identified by angle
/// and tangential position into a pair of detector indexes
class LookupTable {
public:
  LookupTable() : transaxial_table(std::make_unique<TransaxialTableT>()) {
    initialize_tables();
  };

  /// @brief Get the detector pair corresponding to a particular LOR
  /// @param angle_num Projection view number, 0 to 252
  /// @param tang_pos_num Tangential position number, -251 to 252
  /// @return Reference to struct describing the detector pair
  DetectorPair lookup(int32_t angle_num, int32_t tang_pos_num,
                      int32_t sino_num);

private:
  // TODO: Is NSBINS really correct here?
  // It is 344 but there are det_per_ring rows added to the table
  using TransaxialTableT =
      std::array<std::array<std::pair<int32_t, int32_t>, NSBINS>,
                 DETECTORS_PER_RING / 2>;

  using AxialTableT = std::array<std::pair<int32_t, int32_t>, NSINOS>;

  // 2D Array to look up the index of a detector within a ring
  // from the angle number and transaxial bin number
  // size: num_angles, num_tangential
  std::unique_ptr<TransaxialTableT> transaxial_table;

  // 1D Array to look up ring indexes of detector pair by segment number
  std::unique_ptr<AxialTableT> axial_table;

  // Fill the table array with values
  void initialize_tables();

  /// @brief: Fill segment offsets array with differences between ring pairs
  /// TODO: The things inside this should be looped somehow and not repeated
  void fill_ring_differences();

  void fill_axial_positions();

  void fill_transaxial_table();

  // Number of detectors per ring
  const int32_t det_per_ring = DETECTORS_PER_RING;

  std::array<int32_t, NSINOS> axial_positions;

  // Maximum segment number identifying sinogram for span 1
  const int32_t max_segment_num = NSINOS - 1;

  // Segments are labeled according to their ring difference
  // Zeros are direct Sinograms and for the rest the number
  // identifies how far they are tilted and the sign the direction
  std::array<int32_t, NSINOS> ring_differences;

  // Maximum tangential position number
  const int32_t min_tang_pos_num = -(NSBINS / 2 - 1);

  // Minimum tangential position number
  const int32_t max_tang_pos_num = NSBINS / 2;

  // Transform detector number to an index in the table
  //
  // Tangential positions go from -(N_DET/2)+1 to +N_DET/2
  // To store it in the table we need a positive integer
  int32_t tang_pos_to_table_idx(int32_t tang_pos_num);

  // Transform positive integer table index to tangential position number
  // Goes from -(N_DET/2)+1 to +N_DET/2
  int32_t table_idx_to_tang_pos(int32_t table_idx);
};

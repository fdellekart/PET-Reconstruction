#include "Constants.h"
#include <array>
#include <cstdint>
#include <memory>

struct DetectorPair {
  int32_t det_idx_1;
  int32_t det_idx_2;
};

// Lookuptable to transform a LOR identified by angle
// and tangential position into a pair of detector indexes
class LookupTable {
public:
  LookupTable() : table(std::make_unique<TableT>()) { initialize_table(); };

  // Get the detector pair corresponding to a particular LOR
  const DetectorPair &lookup(int32_t angle_num, int32_t tang_pos_num);

private:
  using TableT =
      std::array<std::array<DetectorPair, NSBINS>, DETECTORS_PER_RING / 2>;

  // 2D Array, size: num_angles, num_tangential
  std::unique_ptr<TableT> table;

  // Fill the table array with values
  void initialize_table();

  // Number of detectors per ring
  const int32_t det_per_ring = DETECTORS_PER_RING;

  // Maximum tangential position number
  const int32_t min_tang_pos_num = -(det_per_ring / 2) + 1;

  // Minimum tangential position number
  const int32_t max_tang_pos_num = det_per_ring / 2;

  // Transform detector number to an index in the table
  //
  // Tangential positions go from -(N_DET/2)+1 to +N_DET/2
  // To store it in the table we need a positive integer
  int32_t tang_pos_to_table_idx(int32_t tang_pos_num);

  // Transform positive integer table index to tangential position number
  // Goes from -(N_DET/2)+1 to +N_DET/2
  int32_t table_idx_to_tang_pos(int32_t table_idx);
};

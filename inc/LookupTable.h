#include "Constants.h"
#include <cstdint>

struct DetectorPair {
  int32_t det_idx_1;
  int32_t det_idx_2;
};

// Lookuptable to transform a LOR identified by angle
// and tangential position into a pair of detector indexes
class LookupTable {
public:
  LookupTable() : table() { initialize_table(); };

  // Get the detector pair corresponding to a particular LOR
  DetectorPair lookup(int32_t angle_num, int32_t tang_pos_num);

  // Transform detector number to an index in the table
  //
  // Tangential positions go from -(N_DET/2)+1 to +N_DET/2
  // To store it in the table we need a positive integer
  int32_t tang_pos_to_table_idx(int32_t tang_pos_num);

  // Transform positive integer table index to tangential position number
  // Goes from -(N_DET/2)+1 to +N_DET/2
  int32_t table_idx_to_tang_pos(int32_t table_idx);

private:
  // 2D Array, size: num_angles, num_tangential
  DetectorPair table[DETECTORS_PER_RING / 2][NSBINS];

  // Fill the table array with values
  void initialize_table();

  // Number of detectors per ring
  int32_t det_per_ring = DETECTORS_PER_RING;
};

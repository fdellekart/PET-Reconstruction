#include "LookupTable.h"
#include "Constants.h"
#include <cassert>
#include <cstdint>

int32_t LookupTable::table_idx_to_tang_pos(int32_t table_idx) {
  return table_idx - (det_per_ring / 2) + 1;
};

void LookupTable::initialize_table() {
  int32_t det_per_ring = DETECTORS_PER_RING;
  int32_t arr_idx;
  const int32_t min_tang_pos_num = -(det_per_ring / 2) + 1;
  const int32_t max_tang_pos_num = det_per_ring / 2;

  for (int ang_idx = 0; ang_idx < det_per_ring / 2; ang_idx++) {
    for (int tang_idx = min_tang_pos_num; tang_idx <= max_tang_pos_num;
         tang_idx++) {
      arr_idx = tang_idx + det_per_ring / 2 - 1;
      (*table)[ang_idx][arr_idx].det_idx_1 =
          (ang_idx + tang_idx / 2) % det_per_ring;
      (*table)[ang_idx][arr_idx].det_idx_2 =
          (ang_idx - (tang_idx + 1) / 2 + det_per_ring / 2) % det_per_ring;
    };
  };
};

#include "LookupTable.h"
#include "Constants.h"
#include "Utils.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <format>

int32_t LookupTable::table_idx_to_tang_pos(int32_t table_idx) {
  return table_idx - (NSBINS / 2) + 1;
};

int32_t LookupTable::tang_pos_to_table_idx(int32_t tang_pos_num) {
  return tang_pos_num + (NSBINS / 2) - 1;
};

DetectorPair LookupTable::lookup(int32_t angle_num, int32_t tang_pos_num,
                                 int32_t sino_num) {
  assert(angle_num >= 0 && "Angle number must be positive");
  assert(angle_num <= 251 && "Maximum angle number is 251");
  assert(tang_pos_num >= min_tang_pos_num &&
         "Minimum tangential number is -171");
  assert(tang_pos_num <= max_tang_pos_num &&
         "Maximum tangential number is 171");
  assert(sino_num < 837 && "Maximum index of sinogram is 836");

  int32_t tang_pos_idx = tang_pos_to_table_idx(tang_pos_num);
  std::pair<int32_t, int32_t> positions_in_ring =
      transaxial_table->at(angle_num).at(tang_pos_idx);

  int32_t ring_difference = ring_differences.at(sino_num);
  int32_t axial_position = axial_positions.at(sino_num);

  Detector detector1(positions_in_ring.first,
                     axial_position - (ring_difference / 2));
  Detector detector2(positions_in_ring.second,
                     axial_position + (ring_difference / 2));

  return DetectorPair(detector1, detector2);
};

void LookupTable::initialize_tables() {
  fill_ring_differences();
  fill_transaxial_table();
  fill_axial_positions();
};

void LookupTable::fill_transaxial_table() {
  std::pair<int32_t, int32_t> idxs_in_ring;

  for (int32_t ang_pos_num = 0; ang_pos_num < det_per_ring / 2; ang_pos_num++) {
    for (int32_t tang_pos_num = min_tang_pos_num, tang_arr_idx;
         tang_pos_num <= max_tang_pos_num; tang_pos_num++) {

      tang_arr_idx = tang_pos_to_table_idx(tang_pos_num);
      int32_t first_idx =
          (ang_pos_num + tang_pos_num / 2 + det_per_ring) % det_per_ring;
      int32_t second_idx =
          (ang_pos_num - (tang_pos_num + 1) / 2 + det_per_ring / 2) %
          det_per_ring;

      idxs_in_ring = std::make_pair(first_idx, second_idx);
      transaxial_table->at(ang_pos_num).at(tang_arr_idx) = idxs_in_ring;
    };
  };
};

void LookupTable::fill_ring_differences() {
  auto start = ring_differences.begin();
  std::ranges::fill(start, start + 127, 0);
  start = start + 127;

  assignment_index = 0;
  std::for_each_n(start, 230, assign_if_index_even<1>);
  assignment_index = 0;
  std::for_each_n(start, 230, assign_if_index_odd<-1>);
  start = start + 230;
  assignment_index = 0;

  std::for_each_n(start, 186, assign_if_index_even<2>);
  assignment_index = 0;
  std::for_each_n(start, 186, assign_if_index_odd<-2>);
  start = start + 186;
  assignment_index = 0;

  std::for_each_n(start, 142, assign_if_index_even<3>);
  assignment_index = 0;
  std::for_each_n(start, 142, assign_if_index_odd<-3>);
  start = start + 142;
  assignment_index = 0;

  std::for_each_n(start, 98, assign_if_index_even<4>);
  assignment_index = 0;
  std::for_each_n(start, 98, assign_if_index_odd<-4>);
  start = start + 98;
  assignment_index = 0;

  std::for_each_n(start, 54, assign_if_index_even<5>);
  assignment_index = 0;
  std::for_each_n(start, 54, assign_if_index_odd<-5>);
  assignment_index = 0;

  std::for_each(ring_differences.begin(), ring_differences.end(),
                [](auto &elem) { elem *= SPAN + 1; });
  std::cout << "Filling segment table finished" << std::endl;
};

void assign_ring_positions(int32_t *range, const int32_t n,
                           const int start_ring_idx) {
  for (int i = 0; i < n; i += 2) {
    *(range + i) = start_ring_idx + i / 2;
    *(range + i + 1) = start_ring_idx + i / 2;
  }
};

void LookupTable::fill_axial_positions() {
  int ring_idx = 0;

  auto start = axial_positions.begin();
  std::for_each(start, start + 127, [&ring_idx](auto &elem) {
    elem = ring_idx;
    ring_idx++;
  });
  start += 127;
  ring_idx = 6;
  assign_ring_positions(start, 230, ring_idx);
  start += 230;
  ring_idx += 11;
  assign_ring_positions(start, 186, ring_idx);
  start += 186;
  ring_idx += 11;
  assign_ring_positions(start, 142, ring_idx);
  start += 142;
  ring_idx += 11;
  assign_ring_positions(start, 98, ring_idx);
  start += 98;
  ring_idx += 11;
  assign_ring_positions(start, 54, ring_idx);

  std::cout << "Filling axial positions finished" << std::endl;
};

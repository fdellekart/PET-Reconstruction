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

std::pair<int32_t, int32_t> LookupTable::lookup(int32_t angle_num,
                                                int32_t tang_pos_num) {
  assert(angle_num >= 0 && "Angle number must be positive");
  assert(angle_num <= 251 && "Maximum angle number is 251");
  assert(tang_pos_num >= min_tang_pos_num &&
         "Minimum tangential number is -171");
  assert(tang_pos_num <= max_tang_pos_num &&
         "Maximum tangential number is 171");

  int32_t tang_pos_idx = tang_pos_to_table_idx(tang_pos_num);
  auto pos_in_ring = transaxial_table->at(angle_num).at(tang_pos_idx);
  return pos_in_ring;
};

void LookupTable::initialize_tables() {
  fill_segment_offsets();
  fill_transaxial_table();
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

void LookupTable::fill_segment_offsets() {
  auto start = segment_offsets.begin();
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

  std::cout << "Filling segment table finished" << std::endl;
};

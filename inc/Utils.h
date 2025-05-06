#pragma once

static int assignment_index = 0;

/// @brief Reassign element to VALUE only if the global, 'assignment_index'
/// variable is odd
/// @tparam VALUE value to assign
/// @param element reference whos value will be changed if odd
/// TODO: Reliance on the global variable must be removed
template <int VALUE> void assign_if_index_odd(int32_t &element) {
  if (assignment_index % 2 != 0)
    element = VALUE;
  assignment_index++;
};

/// @brief Reassign element to VALUE only if the global, 'assignment_index'
/// variable is even
/// @tparam VALUE value to assign
/// @param element reference whos value will be changed if even
/// TODO: Reliance on the global variable must be removed
template <int VALUE> void assign_if_index_even(int32_t &element) {
  if (assignment_index % 2 == 0)
    element = VALUE;
  assignment_index++;
};

inline int get_data_index_from_pos(int i, int j, int n_i) {
  return i * n_i + j;
};

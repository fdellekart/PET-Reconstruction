#include <cstdint>

struct Vec2 {
  double x, y;
};

/// @brief Description of a LOR as tangential, angular and projection indexes
struct LOR {
public:
  int32_t tang_idx;
  int32_t angle_idx;
  int32_t proj_idx;
};

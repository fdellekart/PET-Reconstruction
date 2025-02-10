#include <stdexcept>

template <int NANGLES, int NTANG> class Sinogram {
public:
  Sinogram() = default;

  void add_event(int angle_idx, int tang_idx) {
    check_range(angle_idx, tang_idx);
    (data[angle_idx][tang_idx])++;
  };

  int get_bin(int angle_idx, int tang_idx) {
    check_range(angle_idx, tang_idx);
    return data[angle_idx][tang_idx];
  };

private:
  int data[NANGLES][NTANG] = {0};

  void check_range(int angle_idx, int tang_idx) {
    if (angle_idx >= NANGLES) {
      throw std::out_of_range(
          "Angular idx out of range: " + std::to_string(angle_idx) +
          " >= " + std::to_string(NANGLES));
    } else if (tang_idx >= NTANG) {
      throw std::out_of_range(
          "Tangential idx out of range: " + std::to_string(tang_idx) +
          " >= " + std::to_string(NTANG));
    }
  };
};
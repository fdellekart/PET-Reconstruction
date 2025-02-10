template <int NANGLES, int NTANG> class Sinogram {
public:
  Sinogram() = default;

  void add_event(int angle_idx, int tang_idx) {
    (data[angle_idx][tang_idx])++;
  };

  int get_bin(int angle_idx, int tang_idx) {
    return data[angle_idx][tang_idx];
  };

private:
  int data[NANGLES][NTANG] = {0};
};
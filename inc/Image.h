#pragma once
#include "Utils.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

class Image {
public:
  Image() = default;
  Image(int x_dim, int y_dim, double default_value = 0)
      : x_dim(x_dim), y_dim(y_dim) {
    data.resize(x_dim * y_dim);
    std::fill_n(data.begin(), x_dim * y_dim, default_value);
  };

  /// @brief Number of voxels in x direction
  int x_dim;

  /// @brief Number of voxels in y direction
  int y_dim;
  std::vector<double> data = {0};

  bool assert_dim_equal(Image &other) {
    assert(this->x_dim == other.x_dim);
    assert(this->y_dim == other.y_dim);
  };

  void operator*=(Image &other) {
    assert_dim_equal(other);
    for (int i = 0; i < x_dim; i++) {
      for (int j = 0; j < y_dim; j++) {
        data[get_index(i, j)] *= other.data[get_index(i, j)];
      }
    }
  };

  void to_file(const std::string &filepath) {
    std::ofstream outstream(filepath);

    if (!outstream) {
      throw std::runtime_error("Could not open file");
    };

    for (int i = 0; i < x_dim; i++) {
      for (int j = 0; j < y_dim; j++) {
        outstream << std::fixed << std::setprecision(10)
                  << data[get_index(i, j)];
        if (j != y_dim - 1) {
          outstream << ",";
        }
      }
      outstream << std::endl;
    }

    outstream.close();
  };

private:
  int get_index(int x, int y) { return get_data_index_from_pos(x, y, x_dim); };
};
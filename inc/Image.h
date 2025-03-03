#pragma once
#include <fstream>
#include <iomanip>
#include <iostream>

template <int X_DIM, int Y_DIM> class Image {
public:
  Image() = default;
  double data[X_DIM][Y_DIM] = {0};

  void to_file(const std::string &filepath) {
    std::ofstream outstream(filepath);

    if (!outstream) {
      throw std::runtime_error("Could not open file");
    };

    for (int i = 0; i < X_DIM; i++) {
      for (int j = 0; j < Y_DIM; j++) {
        outstream << std::fixed << std::setprecision(10) << data[i][j];
        if (j != Y_DIM - 1) {
          outstream << ",";
        }
      }
      outstream << std::endl;
    }

    outstream.close();
  };
};
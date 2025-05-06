#pragma once
#include "Geometry.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

/// @brief Representation of one projection
template <typename T> class Sinogram {
public:
  Sinogram() = default;
  Sinogram(int n_angular_positions, int n_tangential_positions)
      : n_angular_positions(n_angular_positions),
        n_tangential_positions(n_tangential_positions) {
    data.resize(n_angular_positions * n_tangential_positions);
  };
  Sinogram(const ScannerGeometry *geometry)
      : Sinogram(geometry->n_angular_positions,
                 geometry->n_tangential_positions) {};

  Sinogram operator-(Sinogram<T> &other) {
    Sinogram result;
    for (int i = 0; i < n_angular_positions; i++) {
      for (int j = 0; j < n_tangential_positions; j++) {
        int index = get_index(i, j);
        result.data[index] = data[index] - other.data[index];
      }
    }
    return result;
  }

  Sinogram<T> operator/(Sinogram<T> &other) {
    Sinogram<T> result;
    for (int i = 0; i < n_angular_positions; i++) {
      for (int j = 0; j < n_tangential_positions; j++) {
        int index = get_index(i, j);
        result.data[index] = data[index] / other.data[index];
      }
    }
    return result;
  }

  /// @brief Increment the specified bin by one
  void add_event(int angle_idx, int tang_idx) {
    check_range(angle_idx, tang_idx);
    (data[get_index(angle_idx, tang_idx)])++;
  };

  /// @brief Get the number of events for a bin
  T get_bin(int angle_idx, int tang_idx) {
    check_range(angle_idx, tang_idx);
    return data[get_index(angle_idx, tang_idx)];
  };

  /// @brief Write out the sinogram to a file
  /// one angle per line, tangential indexes separated by commas
  /// @param filepath Absolute path to write to
  void to_file(const std::string &filepath) {
    std::ofstream outstream(filepath);

    if (!outstream) {
      throw std::runtime_error("Could not open file");
    };

    for (int i = 0; i < n_angular_positions; i++) {
      for (int j = 0; j < n_angular_positions; j++) {
        outstream << data[get_index(i, j)];
        if (j != n_tangential_positions - 1) {
          outstream << ",";
        }
      }
      outstream << std::endl;
    }

    outstream.close();
  };

  static Sinogram<int> from_file(const std::string &filepath,
                                 const ScannerGeometry *geometry) {
    Sinogram<int> sinogram(geometry);
    std::ifstream file(filepath);
    std::string line;
    std::string cell;

    for (int i = 0; i < sinogram.n_angular_positions; i++) {
      std::getline(file, line);
      std::stringstream linestream(line);
      for (int j = 0; j < sinogram.n_tangential_positions; j++) {
        std::getline(linestream, cell, ',');
        sinogram
            .data[get_data_index_from_pos(i, j, sinogram.n_angular_positions)] =
            std::stoi(cell);
      }
    }

    return sinogram;
  };

  std::vector<T> data = {0};
  int n_angular_positions;
  int n_tangential_positions;

private:
  /// @brief Check if indexes are in range or throw out_of_range
  void check_range(int angle_idx, int tang_idx) {
    if (angle_idx >= n_angular_positions) {
      throw std::out_of_range(
          "Angular idx out of range: " + std::to_string(angle_idx) +
          " >= " + std::to_string(n_angular_positions));
    } else if (tang_idx >= n_tangential_positions) {
      throw std::out_of_range(
          "Tangential idx out of range: " + std::to_string(tang_idx) +
          " >= " + std::to_string(n_tangential_positions));
    }
  };

  /// @brief Get the index of a datapoint in the vector from angular
  /// and tangential indexes
  int get_index(int ang_idx, int tang_idx) {
    return get_data_index_from_pos(ang_idx, tang_idx, n_angular_positions);
  };
};

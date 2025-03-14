#pragma once
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

/// @brief Representation of one projection
/// @tparam NANGLES Number of angular divisions in the scanner
/// @tparam NTANG Number of possible tangential positions
template <int NANGLES, int NTANG> class Sinogram {
public:
  Sinogram() = default;

  /// @brief Increment the specified bin by one
  void add_event(int angle_idx, int tang_idx) {
    check_range(angle_idx, tang_idx);
    (data[angle_idx][tang_idx])++;
  };

  /// @brief Get the number of events for a bin
  int get_bin(int angle_idx, int tang_idx) {
    check_range(angle_idx, tang_idx);
    return data[angle_idx][tang_idx];
  };

  /// @brief Write out the sinogram to a file
  /// one angle per line, tangential indexes separated by commas
  /// @param filepath Absolute path to write to
  void to_file(const std::string &filepath) {
    std::ofstream outstream(filepath);

    if (!outstream) {
      throw std::runtime_error("Could not open file");
    };

    for (int i = 0; i < NANGLES; i++) {
      for (int j = 0; j < NTANG; j++) {
        outstream << data[i][j];
        if (j != NTANG - 1) {
          outstream << ",";
        }
      }
      outstream << std::endl;
    }

    outstream.close();
  };

  static Sinogram from_file(const std::string &filepath) {
    Sinogram sinogram;
    std::ifstream file(filepath);
    std::string line;
    std::string cell;

    for (int i = 0; i < NANGLES; i++) {
      std::getline(file, line);
      std::stringstream linestream(line);
      for (int j = 0; j < NTANG; j++) {
        std::getline(linestream, cell, ',');
        sinogram.data[i][j] = std::stoi(cell);
      }
    }

    return sinogram;
  };
  double data[NANGLES][NTANG] = {0};

private:
  Sinogram(double *data) : data(data) {};

  /// @brief Check if indexes are in range or throw out_of_range
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
#include "Constants.h"
#include "DataSummary.h"
#include "PETLINKStream.h"
#include "Sinogram.h"
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  auto filename = "/home/florian/Documents/Programming/MMR2PETSIRD/data/LM/"
                  "17598013_1946_20150604155500.000000.bf";
  PETLINKStream input_stream(filename);
  input_stream.seek_time(600);

  if (!input_stream.good()) {
    std::cerr << "File '" << filename << "' not found.";
    exit(EXIT_FAILURE);
  }

  int printed_time = 0;
  int time_from_start = 0; // Time since acquisition start in seconds

  LOR lor;

  Sinogram<252, 344> sinogram;

  for (EventOrTag next : input_stream) {
    if (next.is_event) {
      lor = next.event.get_lor();
      if (next.event.is_prompt & lor.proj_idx == 30) {
        sinogram.add_event(lor.angle_idx, lor.tang_idx);
      };
    } else {
      if (next.tag.is_timetag) {
        time_from_start = next.tag.elapsed_millis / ITIME;

        if (time_from_start > 1000) {
          sinogram.to_file(
              "/home/florian/Documents/Programming/MMR2PETSIRD/sinogram");
          exit(0);
        }
      }
    }
  }
  return 0;
}
#include "Constants.h"
#include "DataSummary.h"
#include "PETLINKStream.h"
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

  std::ofstream log;
  log.open("/home/florian/Documents/Programming/MMR2PETSIRD/log.csv");
  log << "Type, Value\n";

  int printed_time = 0;

  int time_from_start = 0; // Time since acquisition start in seconds
  DataSummary summary;
  int last_val = 0;

  int32_t bin_address;
  int32_t tang_pos_num;
  int32_t rest;
  int32_t angle_num;
  int32_t sino_num;

  for (std::shared_ptr<EventOrTag> next : input_stream) {
    if (next->is_event) {
      bin_address = next->event->bin_address;
      tang_pos_num = bin_address % NSBINS;
      rest = bin_address / NSBINS;
      angle_num = rest % NSANGLES; // view num
      rest = rest / NSANGLES;
      sino_num = rest % NSINOS;
    } else {
      if (next->tag->is_timetag) {
        log << "Time, " << next->tag->elapsed_millis << ","
            << input_stream.tellg() << "\n";
        summary.n_timetags++;
        time_from_start = next->tag->elapsed_millis / ITIME;
        last_val = next->tag->elapsed_millis;
      }
    }

    summary.tot_entries++;

    if (printed_time != time_from_start && ((time_from_start % 1000) == 0)) {
      std::cout << "-------------------------------------\n";
      std::cout << "Time: " << time_from_start << "\n";
      std::cout << "Tangential: " << tang_pos_num << "\n";
      std::cout << "Angle: " << angle_num << "\n";
      std::cout << "Sino: " << sino_num << std::endl;
    }
  }
  log.close();
  return 0;
}
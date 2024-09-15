#include "DataSummary.h"
#include "PETLINKStream.h"
#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_set>

#define ITIME 1000 // Integration time (interval between two time tags?)

int main(int argc, char **argv) {
  auto filename =
      "/home/recon/Documents/Thesis-Dellekart/SIRF-STIR-PET-Reconstruction/"
      "input/1.3.12.2.1107.5.2.38.51035.30000017121607191856600000009.bf";
  PETLINKStream input_stream(filename);
  if (!input_stream.good()) {
    std::cerr << "File '" << filename << "' not found.";
    exit(EXIT_FAILURE);
  }

  int printed_time = 0;

  int time_from_start; // Time since acquisition start in seconds
  DataSummary summary;

  while (input_stream.good()) {
    std::shared_ptr<EventOrTag> next = input_stream.get_next();

    if (next->is_event) {
      if (next->event.is_prompt) {
        summary.n_prompts++;
      } else { // delayeds
        summary.n_delayeds++;
      };
    } else {
      if (next->tag.is_timetag) {
        summary.n_timetags++;
        time_from_start = next->tag.elapsed_millis / ITIME;
      }
    }

    summary.tot_entries++;

    if (printed_time != time_from_start && ((time_from_start % 10) == 0)) {
      std::cout << "-------------------------------------\n";
      std::cout << "Time: " << time_from_start << "\n";
      std::cout << "Prompts: " << summary.n_prompts << "\n";
      std::cout << "Delayeds: " << summary.n_delayeds << "\n";
      std::cout << "Singles: " << summary.n_singles << "\n";
      std::cout << "Timetags: " << summary.n_timetags << "\n";
      std::cout << "Iterations: " << summary.tot_entries << std::endl;
      printed_time = time_from_start;
    }
  }

  return 0;
}
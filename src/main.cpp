#include "DataSummary.h"
#include "PETLINKStream.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_set>

#define ITIME 1000 // Integration time (interval between two time tags?)

int main(int argc, char **argv) {
  auto filename = "/home/florian/Documents/Programming/MMR2PETSIRD/data/LM/"
                  "17598013_1946_20150604155500.000000.bf";
  PETLINKStream input_stream(filename);
  input_stream.seek_time(600);
  exit(EXIT_SUCCESS);

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

  while (input_stream.good()) {
    std::shared_ptr<EventOrTag> next = input_stream.get_next();

    if (next->is_event) {
      log << "Event, " << next->event.bin_address << "\n";
      if (next->event.is_prompt) {
        summary.n_prompts++;
      } else { // delayeds
        summary.n_delayeds++;
      };
    } else {
      if (next->tag.is_timetag) {
        log << "Time, " << next->tag.elapsed_millis << ","
            << input_stream.tellg() << "\n";
        summary.n_timetags++;
        time_from_start = next->tag.elapsed_millis / ITIME;
        last_val = next->tag.elapsed_millis;
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
  log.close();
  return 0;
}
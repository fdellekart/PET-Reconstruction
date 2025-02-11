#include "PETLINKStream.h"
#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <iostream>

int main(int argc, char **argv) {
  if (argc != 5) {
    throw std::runtime_error("extract_timeframe takes exactly four arguments "
                             "for <inpath> <starttime> <endtime> <outpath>");
  }
  PETLINKStream stream(argv[1]);
  int time_start = atoi(argv[2]);
  int time_end = atoi(argv[3]);
  std::ofstream dest_file;
  dest_file.open(argv[4], std::fstream::binary);
  if (!stream.good()) {
    throw std::runtime_error("Problem loading input file!");
  }
  if (!dest_file.good()) {
    throw std::runtime_error("Problem loading destination file!");
  }

  stream.seek_time(time_start);
  auto next_element = stream.get_next();
  assert(!next_element.is_event);
  assert(next_element.tag.is_timetag);
  int time = next_element.tag.elapsed_millis;
  int i = 0;

  int32_t word;
  while (time != time_end) {
    if (next_element.is_event) {
      word = next_element.event.word;
    } else {
      word = next_element.tag.word;
      if (next_element.tag.is_timetag) {
        time = next_element.tag.elapsed_millis;
        if (time % 100 == 0) {
          std::cout << "Processed " << time << " ms" << std::endl;
        }
      }
    }
    dest_file.write(reinterpret_cast<const char *>(&word), sizeof(uint32_t));
    next_element = stream.get_next();
    i++;
  };
  std::cout << "Total words: " << i << std::endl;
  dest_file.close();
}

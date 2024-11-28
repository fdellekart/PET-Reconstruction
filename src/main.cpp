#include <vector>

#include "PETLINKStream.h"

struct FrameResult {
  unsigned long n_prompts = 0;
  unsigned long n_delayeds = 0;
};

int main(int argc, char **argv) {
  auto filename = "/home/florian/Documents/Programming/MMR2PETSIRD/data/LM/"
                  "17598013_1946_20150604155500.000000.bf";

  auto outfile =
      "/home/florian/Documents/Programming/MMR2PETSIRD/events_per_frame.csv";

  PETLINKStream input_stream(filename);

  if (!input_stream.good()) {
    std::cerr << "File '" << filename << "' not found.";
    exit(EXIT_FAILURE);
  }

  unsigned long long prompts = 0;
  unsigned long long delayeds = 0;
  int current_frame = 0;

  std::vector<FrameResult> results;
  results.resize(700);

  for (std::shared_ptr<EventOrTag> next : input_stream) {
    if (next->tag.is_timetag) {
      if (next->tag.elapsed_millis / 1000 > (current_frame + 1) * 30) {
        std::cout << "Frame " << current_frame << " finished with " << prompts
                  << " prompts and " << delayeds << " delayeds." << std::endl;

        results[current_frame].n_prompts = prompts;
        results[current_frame].n_delayeds = delayeds;
        prompts = 0;
        delayeds = 0;
        current_frame++;
      };
      continue;
    }

    if (next->event.is_prompt) {
      prompts++;
    } else {
      delayeds++;
    }
  }

  std::ofstream result_file(outfile);
  result_file << "frame, prompts, delayeds" << "\n";
  current_frame = 0;

  for (FrameResult result : results) {
    result_file << current_frame << "," << result.n_prompts << ","
                << result.n_delayeds << "\n";
    current_frame++;
  }

  result_file.close();
  return 0;
}
#include "Histograming.h"
#include "PETLINKStream.h"
#include <chrono>

int main(int argc, char **argv) {
  PETLINKStream input("/home/florian/Documents/Programming/MMR2PETSIRD/data/LM/"
                      "17598013_1946_20150604155500.000000.bf");
  std::chrono::seconds time_start(1000);
  std::chrono::seconds time_end(1100);
  auto result = histogram<252, 344>(input, time_start, time_end);
  return 0;
}
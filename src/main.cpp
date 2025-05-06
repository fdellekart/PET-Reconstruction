#include "Histograming.h"
#include "PETLINKStream.h"
#include "projections.h"
#include <chrono>

int main(int argc, char **argv) {
  auto geometry = ScannerGeometry::mmr();
  auto start = std::chrono::high_resolution_clock::now();
  PETLINKStream input("/home/florian/Documents/Programming/MMR2PETSIRD/data/LM/"
                      "17598013_1946_20150604155500.000000.bf");
  std::chrono::seconds time_start(0);
  std::chrono::seconds time_end(500);
  auto result = histogram(input, time_start, time_end, &geometry);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Took "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
            << std::endl;
  auto corrected = result.prompts - result.delayeds;
  for (int i = 0; i < 252; i++) {
    for (int j = 0; j < 344; j++) {
      int index = get_data_index_from_pos(i, j, geometry.n_angular_positions);
      if (corrected.data[index] < 0) {
        corrected.data[index] = 0;
      }
    }
  }
  RayTracer tracer({344, 344});
  auto image = project_backward(result.prompts, tracer, geometry);
  image.to_file("/home/florian/Documents/Programming/MMR2PETSIRD/image");
}
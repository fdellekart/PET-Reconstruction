#include "Constants.h"
#include "DataSummary.h"
#include "Image.h"
#include "PETLINKStream.h"
#include "RayTracer.h"
#include "Sinogram.h"
#include "projections.h"
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  Sinogram<252, 344> sinogram = Sinogram<252, 344>::from_file(
      "/home/florian/Documents/Programming/MMR2PETSIRD/sinogram");
  ScannerGeometry geometry = ScannerGeometry::mmr();
  RayTracer tracer(geometry.img_dimensions);
  LOR lor;
  Image<344, 344> image(1.0);
  Sinogram<252, 344> forwardprojection;
  Sinogram<252, 344> errors;
  Image<344, 344> backward_errors;
  char filename[1000];

  auto t1 = std::chrono::high_resolution_clock::now();
  for (int it = 0; it < 20; it++) {
    std::cout << "Iteration " << it << std::endl;

    forwardprojection = project_forward<252, 344>(image, tracer, geometry);
    errors = sinogram / forwardprojection;
    backward_errors = project_backward(errors, tracer, geometry);
    image *= backward_errors;
  }

  snprintf(filename, sizeof(filename),
           "/home/florian/Documents/Programming/MMR2PETSIRD/image_it%d", 20);
  image.to_file(filename);

  auto t2 = std::chrono::high_resolution_clock::now();
  auto delta_t = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

  std::cout << "Processing image took " << delta_t << " ms" << std::endl;
  std::ofstream dur_file(
      "/home/florian/Documents/Programming/MMR2PETSIRD/durations",
      std::ios_base::app);

  dur_file << delta_t << std::endl;

  return 0;
}
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
#include <fstream>
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  Sinogram<252, 344> sinogram = Sinogram<252, 344>::from_file(
      "/home/florian/Documents/Programming/MMR2PETSIRD/sinogram");
  ScannerGeometry geometry = ScannerGeometry::mmr();
  RayTracer tracer(geometry.img_dimensions);
  LOR lor;
  Image<344, 344> image;

  auto t1 = std::chrono::high_resolution_clock::now();
  image = project_backward(sinogram, tracer, geometry);

  auto t2 = std::chrono::high_resolution_clock::now();
  auto delta_t = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

  std::cout << "Processing image took " << delta_t << " ms" << std::endl;
  std::ofstream dur_file(
      "/home/florian/Documents/Programming/MMR2PETSIRD/durations",
      std::ios_base::app);

  dur_file << delta_t << std::endl;

  image.to_file("/home/florian/Documents/Programming/MMR2PETSIRD/image");

  return 0;
}
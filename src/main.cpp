#include "Constants.h"
#include "DataSummary.h"
#include "PETLINKStream.h"
#include "RayTracer.h"
#include "Sinogram.h"
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  Vec2 p1 = {-1.6, 4.5};
  Vec2 p2 = {7.8, 0.4};

  int Nx = 6, Ny = 5;        // Grid size
  double dx = 1.0, dy = 1.0; // Voxel size
  double x0 = 0.0, y0 = 0.0; // Grid origin

  auto hits = siddon_ray_tracing(p1, p2, Nx, Ny, dx, dy, x0, y0);

  for (const auto &hit : hits) {
    std::cout << "Voxel (" << hit.i << ", " << hit.j
              << ") Length: " << hit.length << "\n";
  }

  return 0;
}
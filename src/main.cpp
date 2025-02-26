#include "Constants.h"
#include "DataSummary.h"
#include "Image.h"
#include "PETLINKStream.h"
#include "RayTracer.h"
#include "Sinogram.h"
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  Sinogram<252, 344> sinogram = Sinogram<252, 344>::from_file(
      "/home/florian/Documents/Programming/MMR2PETSIRD/sinogram");
  ScannerGeometry geometry = ScannerGeometry::mmr();
  RayTracer tracer(geometry);
  LOR lor;
  Image<344, 344> image;

  for (int ang_idx = 0; ang_idx < 252; ang_idx++) {
    for (int tang_idx = 0; tang_idx < 344; tang_idx++) {
      lor = LOR(tang_idx, ang_idx, 0);
      auto det_pos = lor.get_det_positions(geometry);
      auto trace = tracer.trace(det_pos.first, det_pos.second);
      for (auto voxel_hit : trace) {
        if (voxel_hit.i < 1 | voxel_hit.j < 1)
          continue; // FIXME
        image.data[voxel_hit.i - 1][voxel_hit.j - 1] +=
            (voxel_hit.length * sinogram.get_bin(ang_idx, tang_idx));
      }
    }
  }

  image.to_file("/home/florian/Documents/Programming/MMR2PETSIRD/image");

  return 0;
}
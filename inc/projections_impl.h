#include "Geometry.h"
#include "Image.h"
#include "RayTracer.h"
#include "Sinogram.h"

template <int N_ANG, int N_TANG>
Image<N_TANG, N_TANG> project_backward(Sinogram<N_ANG, N_TANG> &sinogram,
                                       RayTracer &tracer,
                                       ScannerGeometry &geometry) {
  Image<N_TANG, N_TANG> image;
  LOR lor;

  for (int ang_idx = 0; ang_idx < N_ANG; ang_idx++) {
    for (int tang_idx = 0; tang_idx < N_TANG; tang_idx++) {
      lor = LOR(tang_idx, ang_idx, 0);
      auto det_pos = lor.get_det_positions(geometry);
      auto trace = tracer.trace(det_pos.first, det_pos.second, geometry);
      for (auto voxel_hit : trace) {
        image.data[voxel_hit.i - 1][voxel_hit.j - 1] +=
            (voxel_hit.length * sinogram.get_bin(ang_idx, tang_idx));
      }
    }
  }

  return image;
};

template <int N_ANG, int N_TANG>
Sinogram<N_ANG, N_TANG> project_forward(Image<N_TANG, N_TANG> &image,
                                        RayTracer &tracer,
                                        ScannerGeometry &geometry) {
  Sinogram<N_ANG, N_TANG> sinogram;
  LOR lor;
  for (int ang_idx = 0; ang_idx < N_ANG; ang_idx++) {
    for (int tang_idx = 0; tang_idx < N_TANG; tang_idx++) {
      lor = LOR(tang_idx, ang_idx, 0);
      auto det_pos = lor.get_det_positions(geometry);
      auto trace = tracer.trace(det_pos.first, det_pos.second, geometry);
      double result = 0;
      for (auto hit : trace) {
        result += image.data[hit.i - 1][hit.j - 1] * hit.length;
      }
      sinogram.data[ang_idx][tang_idx] = result;
    }
  }
  return sinogram;
};

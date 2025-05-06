#include "Geometry.h"
#include "Image.h"
#include "RayTracer.h"
#include "Sinogram.h"

Image project_backward(Sinogram &sinogram, RayTracer &tracer,
                       ScannerGeometry &geometry) {
  Image image(sinogram.n_tangential_positions, sinogram.n_tangential_positions,
              0);
  LOR lor;

  for (int ang_idx = 0; ang_idx < sinogram.n_tangential_positions; ang_idx++) {
    for (int tang_idx = 0; tang_idx < sinogram.n_tangential_positions;
         tang_idx++) {
      lor = LOR(tang_idx, ang_idx, 0);
      auto det_pos = lor.get_det_positions(geometry);
      auto trace = tracer.trace(det_pos.first, det_pos.second, geometry);
      for (auto voxel_hit : trace) {
        int index = get_data_index_from_pos(voxel_hit.i - 1, voxel_hit.j - 1,
                                            image.x_dim);
        image.data[index] +=
            (voxel_hit.length * sinogram.get_bin(ang_idx, tang_idx));
      }
    }
  }

  return image;
};

Sinogram project_forward(Image &image, RayTracer &tracer,
                         ScannerGeometry *geometry) {
  Sinogram sinogram(geometry);
  LOR lor;
  for (int ang_idx = 0; ang_idx < sinogram.n_angular_positions; ang_idx++) {
    for (int tang_idx = 0; tang_idx < sinogram.n_tangential_positions;
         tang_idx++) {
      lor = LOR(tang_idx, ang_idx, 0);
      auto det_pos = lor.get_det_positions(*geometry);
      auto trace = tracer.trace(det_pos.first, det_pos.second, *geometry);
      double result = 0;
      for (auto hit : trace) {
        int index = get_data_index_from_pos(hit.i - 1, hit.j - 1, image.x_dim);
        result += image.data[index] * hit.length;
      }
      sinogram.data[get_data_index_from_pos(
          ang_idx, tang_idx, sinogram.n_angular_positions)] = result;
    }
  }
  return sinogram;
};

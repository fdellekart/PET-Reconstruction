#include "RayTracer.h"

std::vector<VoxelHit> RayTracer::trace(Vec2<double> ray_start,
                                       Vec2<double> ray_end) {
  // Implementation of Siddon's algorithm
  auto Nx = geometry.img_dimensions.x + 1;
  auto Ny = geometry.img_dimensions.y + 1;

  ray_start.x += geometry.origin.x;
  ray_end.x += geometry.origin.x;
  ray_start.y += geometry.origin.y;
  ray_end.y += geometry.origin.y;

  auto get_alpha_x = [ray_start, ray_end, this](int i) {
    return ((i - 1) * this->geometry.voxel_size - ray_start.x) /
           (ray_end.x - ray_start.x);
  };

  auto get_alpha_y = [ray_start, ray_end, this](int j) {
    return ((j - 1) * this->geometry.voxel_size - ray_start.y) /
           (ray_end.y - ray_start.y);
  };

  bool x_equal = std::abs(ray_end.x - ray_start.x) < 0.0001;
  bool y_equal = std::abs(ray_end.y - ray_start.y) < 0.0001;

  double alpha_min =
      !x_equal ? std::max(0.0, std::min(get_alpha_x(1), get_alpha_x(Nx))) : 0.0;
  alpha_min =
      !y_equal ? std::max(alpha_min, std::min(get_alpha_y(1), get_alpha_y(Ny)))
               : alpha_min;

  double alpha_max =
      !x_equal ? std::min(1.0, std::max(get_alpha_x(1), get_alpha_x(Nx))) : 1.0;
  alpha_max =
      !y_equal ? std::min(alpha_max, std::max(get_alpha_y(1), get_alpha_y(Ny)))
               : alpha_max;

  assert(alpha_max > alpha_min); // The ray does not intersect the volume

  auto alpha_for_min = ray_end.x > ray_start.x ? alpha_min : alpha_max;
  auto alpha_for_max = ray_end.x > ray_start.x ? alpha_max : alpha_min;

  int i_min = Nx - (geometry.voxel_size * (Nx - 1) -
                    alpha_for_min * (ray_end.x - ray_start.x) - ray_start.x) /
                       geometry.voxel_size;
  int i_max = 1 + (ray_start.x + alpha_for_max * (ray_end.x - ray_start.x)) /
                      geometry.voxel_size;

  alpha_for_min = ray_end.y > ray_start.y ? alpha_min : alpha_max;
  alpha_for_max = ray_end.y > ray_start.y ? alpha_max : alpha_min;

  int j_min = Ny - (geometry.voxel_size * (Ny - 1) -
                    alpha_for_min * (ray_end.y - ray_start.y) - ray_start.y) /
                       geometry.voxel_size;
  int j_max = 1 + (ray_start.y + alpha_for_max * (ray_end.y - ray_start.y)) /
                      geometry.voxel_size;

  alpha_x.resize(i_max - i_min);
  alpha_y.resize(j_max - j_min);
  int dir;

  dir = ray_start.x <= ray_end.x ? 1 : -1;

  assert(i_min <= i_max);
  assert(j_min <= j_max);

  for (int i = i_min; i < i_max; i++) {
    int idx = i - i_min;
    idx = dir > 0 ? idx : i_max - i_min - idx - 1;
    alpha_x[idx] = get_alpha_x(i + 1);
  }

  dir = ray_start.y <= ray_end.y ? 1 : -1;

  for (int j = j_min; j < j_max; j++) {
    int idx = j - j_min;
    idx = dir > 0 ? idx : j_max - j_min - idx - 1;
    alpha_y[idx] = get_alpha_y(j + 1);
  }

  int n = (i_max - i_min + 1) + (j_max - j_min + 1);

  alphas.resize(n);
  alphas[0] = alpha_min;
  alphas[n - 1] = alpha_max;
  std::merge(alpha_x.begin(), alpha_x.end(), alpha_y.begin(), alpha_y.end(),
             ++alphas.begin());

  std::vector<VoxelHit> hits(n - 1);

  for (int i = 0; i < n - 1; i++) {
    hits[i] = VoxelHit(1 + (ray_start.x + ((alphas[i + 1] + alphas[i]) / 2) *
                                              (ray_end.x - ray_start.x)) /
                               geometry.voxel_size,
                       1 + (ray_start.y + ((alphas[i + 1] + alphas[i]) / 2) *
                                              (ray_end.y - ray_start.y)) /
                               geometry.voxel_size,
                       alphas[i + 1] - alphas[i]);
  }

  return hits;
};

#include "Geometry.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

struct VoxelHit {
  int i, j;      // Voxel indices
  double length; // Path length in the voxel
};

// Siddon's algorithm to compute path lengths in voxels
std::vector<VoxelHit> siddon_ray_tracing(Vec2<double> ray_start,
                                         Vec2<double> ray_end, int Nx, int Ny,
                                         double dx, double dy, double x0,
                                         double y0) {
  std::vector<VoxelHit> hits;

  Nx++;
  Ny++;

  ray_start.x += x0;
  ray_end.x += x0;
  ray_start.y += y0;
  ray_end.y += y0;

  auto get_alpha_x = [ray_start, ray_end, x0, dx](int i) {
    return ((i - 1) * dx - ray_start.x) / (ray_end.x - ray_start.x);
  };

  auto get_alpha_y = [ray_start, ray_end, y0, dy](int j) {
    return ((j - 1) * dy - ray_start.y) / (ray_end.y - ray_start.y);
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

  int i_min = Nx - (dx * (Nx - 1) - alpha_for_min * (ray_end.x - ray_start.x) -
                    ray_start.x) /
                       dx;
  int i_max =
      1 + (ray_start.x + alpha_for_max * (ray_end.x - ray_start.x)) / dx;

  alpha_for_min = ray_end.y > ray_start.y ? alpha_min : alpha_max;
  alpha_for_max = ray_end.y > ray_start.y ? alpha_max : alpha_min;

  int j_min = Ny - (dy * (Ny - 1) - alpha_for_min * (ray_end.y - ray_start.y) -
                    ray_start.y) /
                       dy;
  int j_max =
      1 + (ray_start.y + alpha_for_max * (ray_end.y - ray_start.y)) / dy;

  std::vector<double> alpha_x(std::abs(i_max - i_min));
  std::vector<double> alpha_y(std::abs(j_max - j_min));

  int start = i_min < i_max ? i_min : i_max;
  int end = i_min < i_max ? i_max : i_min;

  for (int i = start; i < end; i++) {
    alpha_x[i - start] = get_alpha_x(i + 1);
  }

  start = j_min < j_max ? j_min : j_max;
  end = j_min < j_max ? j_max : j_min;

  for (int j = start; j < end; j++) {
    alpha_y[j - start] = get_alpha_y(j + 1);
  }

  int n = (i_max - i_min + 1) + (j_max - j_min + 1);

  if (ray_start.x > ray_end.x)
    std::reverse(alpha_x.begin(), alpha_x.end());
  if (ray_start.y > ray_end.y)
    std::reverse(alpha_y.begin(), alpha_y.end());

  std::vector<double> alphas(n);
  alphas[0] = alpha_min;
  alphas[n - 1] = alpha_max;
  std::merge(alpha_x.begin(), alpha_x.end(), alpha_y.begin(), alpha_y.end(),
             ++alphas.begin());

  for (int i = 0; i < n - 1; i++) {
    hits.push_back(
        VoxelHit(1 + (ray_start.x + ((alphas[i + 1] + alphas[i]) / 2) *
                                        (ray_end.x - ray_start.x)) /
                         dx,
                 1 + (ray_start.y + ((alphas[i + 1] + alphas[i]) / 2) *
                                        (ray_end.y - ray_start.y)) /
                         dy,
                 alphas[i + 1] - alphas[i]));
  }

  return hits;
}

class RayTracer {
public:
  /// @param img_dimension Number of voxels for the image in each direction
  /// @param voxel_size Voxel sizes for each direction
  /// @param origin Origin of the coordinate system relative to lower left image
  /// corner
  RayTracer(ScannerGeometry geometry) : geometry(geometry) {};

  /// @brief Trace the line connecting the points through the image
  /// @param ray_start Start point of line
  /// @param ray_end End point of line
  /// @return Vector of voxel hits with intersected 1-based voxel indices and
  /// intersection lengths

  /// FIXME: Currently sometimes includes voxels outside the image with
  ///        a length of 0.
  /// TODO: Adapting to 0-based indexing could be good
  std::vector<VoxelHit> trace(const Vec2<double> &ray_start,
                              const Vec2<double> &ray_end) {
    return siddon_ray_tracing(ray_start, ray_end, geometry.img_dimensions.x,
                              geometry.img_dimensions.y, geometry.voxel_size,
                              geometry.voxel_size, geometry.origin.x,
                              geometry.origin.y);
  };

private:
  ScannerGeometry geometry;
};

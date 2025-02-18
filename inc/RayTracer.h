#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

struct Vec2 {
  double x, y;
};

struct VoxelHit {
  int i, j;      // Voxel indices
  double length; // Path length in the voxel
};

// Siddon's algorithm to compute path lengths in voxels
std::vector<VoxelHit> siddon_ray_tracing(const Vec2 &ray_start,
                                         const Vec2 &ray_end, int Nx, int Ny,
                                         double dx, double dy, double x0,
                                         double y0) {
  std::vector<VoxelHit> hits;

  Nx++;
  Ny++;

  auto get_alpha_x = [ray_start, ray_end, x0, dx](int i) {
    return (x0 + (i - 1) * dx - ray_start.x) / (ray_end.x - ray_start.x);
  };

  auto get_alpha_y = [ray_start, ray_end, y0, dy](int j) {
    return (y0 + (j - 1) * dy - ray_start.y) / (ray_end.y - ray_start.y);
  };

  double alpha_min =
      std::max(std::max(0.0, std::min(get_alpha_x(1), get_alpha_x(Nx))),
               std::min(get_alpha_y(1), get_alpha_y(Ny)));
  double alpha_max =
      std::min(std::min(1.0, std::max(get_alpha_x(1), get_alpha_x(Nx))),
               std::max(get_alpha_y(1), get_alpha_y(Ny)));
  assert(alpha_max > alpha_min); // The ray does not intersect the volume

  auto alpha_for_min = ray_end.x > ray_start.x ? alpha_min : alpha_max;
  auto alpha_for_max = ray_end.x > ray_start.x ? alpha_max : alpha_min;

  int i_min = Nx - (x0 + dx * (Nx - 1) -
                    alpha_for_min * (ray_end.x - ray_start.x) - ray_start.x) /
                       dx;
  int i_max =
      1 + (ray_start.x + alpha_for_max * (ray_end.x - ray_start.x) - x0) / dx;

  alpha_for_min = ray_end.y > ray_start.y ? alpha_min : alpha_max;
  alpha_for_max = ray_end.y > ray_start.y ? alpha_max : alpha_min;

  int j_min = Ny - (y0 + dy * (Ny - 1) -
                    alpha_for_min * (ray_end.y - ray_start.y) - ray_start.y) /
                       dy;
  int j_max =
      1 + (ray_start.y + alpha_for_max * (ray_end.y - ray_start.y) - y0) / dy;

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
    hits.push_back(VoxelHit(
        1 + (ray_start.x +
             ((alphas[i + 1] + alphas[i]) / 2) * (ray_end.x - ray_start.x) -
             x0) /
                dx,
        1 + (ray_start.y +
             ((alphas[i + 1] + alphas[i]) / 2) * (ray_end.y - ray_start.y) -
             y0) /
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
  RayTracer(Vec2 img_dimension, Vec2 voxel_size, Vec2 origin)
      : img_dimension(img_dimension), voxel_size(voxel_size), origin(origin) {};

  /// @brief Trace the line connecting the points through the image
  /// @param ray_start Start point of line
  /// @param ray_end End point of line
  /// @return Vector of voxel hits with intersected 1-based voxel indices and
  /// intersection lengths

  /// FIXME: Currently sometimes includes voxels outside the image with
  ///        a length of 0.
  /// TODO: Adapting to 0-based indexing could be good
  std::vector<VoxelHit> trace(const Vec2 &ray_start, const Vec2 &ray_end) {
    return siddon_ray_tracing(ray_start, ray_end, img_dimension.x,
                              img_dimension.y, voxel_size.x, voxel_size.y,
                              origin.x, origin.y);
  };

private:
  Vec2 img_dimension;
  Vec2 voxel_size;
  Vec2 origin;
};

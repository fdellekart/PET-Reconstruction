#include "Geometry.h"
#include <cmath>

#define M_PI 3.14159265358979323846

std::pair<Vec2<double>, Vec2<double>>
LOR::get_det_positions(const ScannerGeometry &geometry) {
  std::pair<Vec2<double>, Vec2<double>> result;

  double angle = (M_PI / (geometry.det_per_ring / 2)) * angle_idx;
  double k = tan(angle);

  double d = -(geometry.detector_diameter / 2) / cos(angle) +
             geometry.voxel_size * (0.5 + tang_idx) / cos(angle);

  double p = (2 * k * d) / (1 + pow(k, 2));
  double q =
      (pow(d, 2) - pow(geometry.detector_diameter / 2, 2)) / (1 + pow(k, 2));

  result.first.x = -(p / 2) + sqrt(pow(p / 2, 2) - q);
  result.second.x = -(p / 2) - sqrt(pow(p / 2, 2) - q);
  result.first.y = k * result.first.x + d;
  result.second.y = k * result.second.x + d;

  result.first.x += geometry.detector_diameter / 2 - geometry.origin.x;
  result.second.x += geometry.detector_diameter / 2 - geometry.origin.x;

  result.first.y += geometry.detector_diameter / 2 - geometry.origin.y;
  result.second.y += geometry.detector_diameter / 2 - geometry.origin.y;

  return result;
};

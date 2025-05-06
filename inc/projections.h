#pragma once
#include "Image.h"
#include "RayTracer.h"
#include "Sinogram.h"
#include "projections_impl.h"

Image<double> project_backward(Sinogram<int> &sinogram, RayTracer &tracer,
                               ScannerGeometry &geometry);

Sinogram<double> project_forward(Image<double> &image, RayTracer &tracer,
                                 ScannerGeometry &geometry);

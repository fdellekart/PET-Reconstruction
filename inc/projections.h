#pragma once
#include "Image.h"
#include "RayTracer.h"
#include "Sinogram.h"
#include "projections_impl.h"

Image project_backward(Sinogram &sinogram, RayTracer &tracer,
                       ScannerGeometry &geometry);

Sinogram project_forward(Image &image, RayTracer &tracer,
                         ScannerGeometry &geometry);

#pragma once
#include "Image.h"
#include "RayTracer.h"
#include "Sinogram.h"
#include "projections_impl.h"

template <int N_ANG, int N_TANG>
Image<N_TANG, N_TANG> project_backward(Sinogram &sinogram, RayTracer &tracer,
                                       ScannerGeometry &geometry);

template <int N_ANG, int N_TANG>
Sinogram project_forward(Image<N_TANG, N_TANG> &image, RayTracer &tracer,
                         ScannerGeometry &geometry);

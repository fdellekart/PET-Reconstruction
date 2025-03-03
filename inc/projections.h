#pragma once
#include "Image.h"
#include "RayTracer.h"
#include "Sinogram.h"
#include "projections_impl.h"

template <int N_ANG, int N_TANG>
Image<N_TANG, N_TANG> project_backward(Sinogram<N_ANG, N_TANG> &sinogram,
                                       RayTracer &ray_tracer,
                                       ScannerGeometry &geometry);

template <int N_ANG, int N_TANG>
Sinogram<N_ANG, N_TANG> project_forward(Image<N_TANG, N_TANG> &image,
                                        RayTracer &ray_tracer,
                                        ScannerGeometry &geometry);

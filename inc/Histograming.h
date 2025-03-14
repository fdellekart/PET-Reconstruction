#pragma once
#include "PETLINKStream.h"
#include "Sinogram.h"

template <int NANGLES, int NTANG> struct EmissionData {
  Sinogram<NANGLES, NTANG> prompts;
  Sinogram<NANGLES, NTANG> delayeds;
};

/// @brief Histogram list mode data
/// @param stream Input stream of list mode data
/// @param start_time Start time in seconds
/// @param end_time End time in seconds
/// @return Prompt and delayed sinograms
template <int NANGLES, int NTANG>
EmissionData<NANGLES, NTANG> histogram(PETLINKStream &stream, int start_time,
                                       int end_time);

#include "Histograming_impl.h"

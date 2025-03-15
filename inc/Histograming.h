#pragma once
#include "PETLINKStream.h"
#include "Sinogram.h"
#include <chrono>

template <int NANGLES, int NTANG> struct EmissionData {
  Sinogram<NANGLES, NTANG> prompts;
  Sinogram<NANGLES, NTANG> delayeds;
};

/// @brief Histogram list mode data
/// @param stream Input stream of list mode data
/// @param start_time Start time relative to acquisition start
/// @param end_time End time relative to acquisition start
/// @return Prompt and delayed sinograms
template <int NANGLES, int NTANG>
EmissionData<NANGLES, NTANG> histogram(PETLINKStream &stream,
                                       std::chrono::seconds start_time,
                                       std::chrono::seconds end_time);

template <int NANGLES, int NTANG>
EmissionData<NANGLES, NTANG> histogram(PETLINKStream &stream,
                                       std::chrono::milliseconds start_time,
                                       std::chrono::milliseconds end_time);

#include "Histograming_impl.h"

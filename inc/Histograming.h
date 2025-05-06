#pragma once
#include "Geometry.h"
#include "PETLINKStream.h"
#include "Sinogram.h"
#include <chrono>

template <typename T> class EmissionData {
public:
  EmissionData() = delete;
  EmissionData(ScannerGeometry *geometry)
      : geometry(geometry), prompts(geometry), delayeds(geometry) {};

  Sinogram<T> prompts;
  Sinogram<T> delayeds;
  ScannerGeometry *geometry;
};

/// @brief Histogram list mode data
/// @param stream Input stream of list mode data
/// @param start_time Start time relative to acquisition start
/// @param end_time End time relative to acquisition start
/// @return Prompt and delayed sinograms
EmissionData<int> histogram(PETLINKStream &stream,
                            std::chrono::seconds start_time,
                            std::chrono::seconds end_time,
                            ScannerGeometry *geometry);

EmissionData<int> histogram(PETLINKStream &stream,
                            std::chrono::milliseconds start_time,
                            std::chrono::milliseconds end_time,
                            ScannerGeometry *geometry);

#include "Histograming_impl.h"

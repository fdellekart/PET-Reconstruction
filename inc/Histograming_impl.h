#pragma once
#include "Constants.h"
#include "Geometry.h"
#include "Histograming.h"
#include "PETLINKStream.h"
#include "Sinogram.h"
#include <chrono>
#include <variant>

#define ms_cast std::chrono::duration_cast<std::chrono::milliseconds>

template <int NANGLES, int NTANG>
EmissionData<NANGLES, NTANG> histogram(PETLINKStream &stream,
                                       std::chrono::seconds start_time,
                                       std::chrono::seconds end_time) {
  return histogram<NANGLES, NTANG>(stream, ms_cast(start_time),
                                   ms_cast(end_time));
}

template <int NANGLES, int NTANG>
EmissionData<NANGLES, NTANG> histogram(PETLINKStream &stream,
                                       std::chrono::milliseconds start_time,
                                       std::chrono::milliseconds end_time) {
  EmissionData<NANGLES, NTANG> result;
  stream.seek_time(start_time);
  LOR lor;

  for (auto next : stream) {
    if (std::holds_alternative<Event>(next)) {
      auto event = std::get<Event>(next);
      lor = event.get_lor();
      if (lor.proj_idx != 30)
        continue;
      if (event.is_prompt) {
        result.prompts.add_event(lor.angle_idx, lor.tang_idx);
      } else {
        result.delayeds.add_event(lor.angle_idx, lor.tang_idx);
      }
    } else {
      auto tag = std::get<Tag>(next);
      if (tag.is_timetag & (tag.time > end_time))
        break;
    }
  }

  return result;
};

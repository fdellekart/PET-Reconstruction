#pragma once
#include "Constants.h"
#include "Geometry.h"
#include "Histograming.h"
#include "PETLINKStream.h"
#include "Sinogram.h"

template <int NANGLES, int NTANG>
EmissionData<NANGLES, NTANG> histogram(PETLINKStream &stream, int start_time,
                                       int end_time) {
  EmissionData<NANGLES, NTANG> result;
  stream.seek_time(start_time);
  LOR lor;

  for (EventOrTag next : stream) {
    if (next.is_event) {
      lor = next.event.get_lor();
      if (lor.proj_idx != 30)
        continue;
      if (next.event.is_prompt) {
        result.prompts.add_event(lor.angle_idx, lor.tang_idx);
      } else {
        result.delayeds.add_event(lor.angle_idx, lor.tang_idx);
      }
    } else if (next.tag.is_timetag &
               (next.tag.elapsed_millis / ITIME) > end_time) {
      break;
    }
  }

  return result;
};

#pragma once

#include "Time.h"

class Stats {
public:
  Stats(Stats const&) = delete;
  void operator=(Stats const&) = delete;

  void reset() {
    num_triangles = 0;
    num_objects = 0;
    time_to_render_ms = 0;
    time_to_update_ms = 0;
    frames_rendered = 0;
    reset_time = Clock::now();
  }

  static Stats& getInstance() {
    static Stats instance;
    return instance;
  }

  int num_triangles = 0;
  int num_objects = 0;
  int time_to_render_ms = 0;
  int time_to_update_ms = 0;
  int frames_rendered = 0;
  TimePoint reset_time;


private:
  Stats() {}
};
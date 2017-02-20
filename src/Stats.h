#pragma once

class Stats {
public:
  Stats(Stats const&) = delete;
  void operator=(Stats const&) = delete;

  void reset() {
    num_triangles = 0;
    time_to_render_ms = 0;
    time_to_update_ms = 0;
  }

  static Stats& getInstance() {
    static Stats instance;
    return instance;
  }

  int num_triangles = 0;
  int time_to_render_ms = 0;
  int time_to_update_ms = 0;


private:
  Stats() {}
};
#pragma once

class Stats {
public:
  Stats(Stats const&) = delete;
  void operator=(Stats const&) = delete;

  void reset() {
    num_triangles = 0;
  }

  static Stats& getInstance() {
    static Stats instance;
    return instance;
  }

  int num_triangles = 0;

private:
  Stats() {}
};
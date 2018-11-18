#include "./Time.h"

int toMs(Duration duration) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

Duration measureFunction(std::function<void()> f) {
  TimePoint start = Clock::now();
  f();
  return Clock::now() - start;
}
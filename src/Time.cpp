#include "Time.h"

int toMs(Duration duration) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}
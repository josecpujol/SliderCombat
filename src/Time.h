#include <chrono>

using namespace std::literals::chrono_literals;
using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
using Duration = Clock::duration;

int toMs(Duration);
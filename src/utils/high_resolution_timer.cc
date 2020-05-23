#include <utils/high_resolution_timer.h>
using namespace DR;

HRTimer::HRTimer() { start_ = end_ = clock_::now(); }

void HRTimer::end() { end_ = clock_::now(); }

double HRTimer::elapsed() const {
  return std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_)
      .count();
}

void HRTimer::reset() { start_ = end_ = clock_::now(); }

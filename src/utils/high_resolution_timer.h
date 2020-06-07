#pragma once
#include <chrono>
#include <utils/di_global.h>

NAMESPACE_BEGIN(DR)

class HRTimer {
public:
  HRTimer();
  void start() { start_ = clock_::now(); }

  void end();

  double elapsed() const;
  void reset();

private:
  using clock_ = std::chrono::high_resolution_clock;
  decltype(clock_::now()) start_;
  decltype(clock_::now()) end_;
};
NAMESPACE_END(DR)

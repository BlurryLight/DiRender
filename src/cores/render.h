#pragma once
#include <cores/scene.h>
#include <math/vector.hpp>
#include <utils/thread_pool.hpp>
#include <vector>
NAMESPACE_BEGIN(DR)
class Render {
public:
  Render(uint spp) : spp_(spp) {}
  Render(uint spp, int nthreads) : spp_(spp), pool_(nthreads) {}
  void render(const Scene &scene);

private:
  uint spp_ = 32;
  DR::ThreadPool pool_;
};

NAMESPACE_END(DR)

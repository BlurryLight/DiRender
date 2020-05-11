#pragma once
#include <utils/thread_pool.hpp>
#include <math/vector.hpp>
#include <vector>
class Render
{
public:
  void render();
private:
  DR::ThreadPool pool_;
  std::vector<DR::Vector3f> framebuffer_;
};

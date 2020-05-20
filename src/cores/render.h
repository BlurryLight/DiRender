#pragma once
#include <math/vector.hpp>
#include <utils/thread_pool.hpp>
#include <vector>
class Render {
public:
  void render();

private:
  DR::ThreadPool pool_;
};

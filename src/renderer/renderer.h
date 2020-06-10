#pragma once
#include <cores/scene.h>
#include <math/vector.hpp>
#include <mutex>
#include <utils/thread_pool.hpp>
NAMESPACE_BEGIN(DR)
class Renderer {
public:
  Renderer(uint spp) : spp_(spp) {}
  Renderer(uint spp, int nthreads) : spp_(spp), pool_(nthreads) {}
  // Renderer is not copyable, movable or assignable
  Renderer(const Renderer &other) = delete;
  Renderer(Renderer &&other) = delete;
  Renderer &operator=(const Renderer &other) = delete;
  virtual void render(const Scene &scene) = 0;
  virtual ~Renderer() {}
  virtual void UpdateProgrss(int num_of_tiles) final {
    std::unique_lock<std::mutex> lk(mutex_);
    count_++;
    std::cout << "Tiles Complete:" << count_ << " Total: " << num_of_tiles
              << " " << (float)count_ / num_of_tiles * 100.0 << "%" << '\r';
    fflush(stdout);
  }

protected:
  uint spp_ = 32;
  DR::ThreadPool pool_;
  int count_ = 0;
  std::mutex mutex_;
};

NAMESPACE_END(DR)

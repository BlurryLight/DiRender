//
// Created by zhong on 2021/3/28.
//

#ifndef DIRENDER_SAMPLER_HH
#define DIRENDER_SAMPLER_HH
#include <utility>
#include <utils/di_global.h>
namespace DR {

// Warning: All sampler should be thread-safe
class Sampler {
public:
  // be cautious: child class will not inherit the default value!
  virtual float get_1d() = 0;
  virtual std::pair<float, float> get_2d() = 0;
  virtual float get_1d(float min, float max) = 0;
  virtual std::pair<float, float> get_2d(float min, float max) = 0;
  virtual ~Sampler() = default;
  Sampler() = default;
  Sampler(const Sampler &) = delete;
  Sampler &operator=(const Sampler &) = delete;
};
} // namespace DR

#endif // DIRENDER_SAMPLER_HH

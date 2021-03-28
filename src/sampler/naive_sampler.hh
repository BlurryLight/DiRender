//
// Created by zhong on 2021/3/28.
//

#ifndef DIRENDER_NAIVE_SAMPLER_HH
#define DIRENDER_NAIVE_SAMPLER_HH
#include <random>
#include <sampler/sampler.hh>
namespace DR {
class NaiveSampler : public Sampler {
public:
  NaiveSampler();
  float get_1d() override;
  std::pair<float, float> get_2d() override;
  float get_1d(float min, float max) override;
  std::pair<float, float> get_2d(float min, float max) override;

private:
  thread_local static std::mt19937 gen1_;
  std::uniform_real_distribution<float> dis_;
};
} // namespace DR

#endif // DIRENDER_NAIVE_SAMPLER_HH

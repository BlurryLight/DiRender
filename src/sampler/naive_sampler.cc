//
// Created by zhong on 2021/3/28.
//

#include "naive_sampler.hh"
using namespace DR;
thread_local std::mt19937
    NaiveSampler::gen1_(std::mt19937(std::random_device{}()));
NaiveSampler::NaiveSampler() : dis_(0.0f, 1.0f) {}
float NaiveSampler::get_1d(float min, float max) {
  std::uniform_real_distribution<> dis(min, max);
  return dis(gen1_);
}

std::pair<float, float> NaiveSampler::get_2d(float min, float max) {
  std::uniform_real_distribution<> dis(min, max);
  return {dis(gen1_), dis(gen1_)};
}
float NaiveSampler::get_1d() { return dis_(gen1_); }
std::pair<float, float> NaiveSampler::get_2d() {
  return {dis_(gen1_), dis_(gen1_)};
}

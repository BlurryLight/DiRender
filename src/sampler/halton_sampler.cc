//
// Created by zhong on 2021/3/30.
//

#include "halton_sampler.hh"
float DR::HaltonSampler::get_1d() { return h1.get(); }
float DR::HaltonSampler::get_1d(float min, float max) {
  return (max - min) * h1.get() + min;
}
std::pair<float, float> DR::HaltonSampler::get_2d() {
  return {get_1d(), get_1d()};
}
std::pair<float, float> DR::HaltonSampler::get_2d(float min, float max) {
  return {get_1d(min, max), get_1d(min, max)};
}

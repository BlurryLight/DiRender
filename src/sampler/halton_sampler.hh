//
// Created by zhong on 2021/3/30.
//

#ifndef DIRENDER_HALTONSAMPLER_HH
#define DIRENDER_HALTONSAMPLER_HH
#include <sampler/sampler.hh>
namespace DR {
class HaltonSampler : public Sampler {
private:
  struct Halton {
    int base = 2, index = 0;
    float get() {
      auto tmp = radical_inverse();
      index++;
      return tmp;
    }

  private:
    float radical_inverse() const {
      float f = 1.0f / base;
      float inv_base = 1.0f / base;
      float value = 0.0f;
      int i = index;
      while (i > 0) {
        value += f * (float)(i % base);
        i /= base;
        f *= inv_base;
      }
      return value;
    }
  };

  Halton h1{2, 0};
  Halton h2{3, 0};

public:
  float get_1d() override;
  float get_1d(float min, float max) override;
  std::pair<float, float> get_2d() override;
  std::pair<float, float> get_2d(float min, float max) override;
};
} // namespace DR

#endif // DIRENDER_HALTONSAMPLER_HH

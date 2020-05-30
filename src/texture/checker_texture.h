#pragma once
#include <texture/constant_texture.h>
#include <texture/texture.h>
NAMESPACE_BEGIN(DR)
class CheckerTexture : public Texture {
public:
  CheckerTexture(const Vector3f &odd, const Vector3f &even, float scale = 5)
      : odd_{std::make_unique<ConstantTexture>(odd)},
        even_{std::make_unique<ConstantTexture>(even)}, scale_(scale) {}
  CheckerTexture()
      : odd_{std::make_unique<ConstantTexture>(Vector3f{0.0f})},
        even_{std::make_unique<ConstantTexture>(Vector3f{1.0f})} {}
  Vector3f evalValue(const Intersection &isect) const override {
    auto uv = isect.texcoords;
    float pattern =
        (fmodf(uv.x * scale_, 1) > 0.5) ^ (fmodf(uv.y * scale_, 1) > 0.5);
    return lerp(odd_->evalValue(isect), even_->evalValue(isect), pattern);
  }

private:
  std::unique_ptr<ConstantTexture> odd_;
  std::unique_ptr<ConstantTexture> even_;
  float scale_ = 5;
};
NAMESPACE_END(DR)

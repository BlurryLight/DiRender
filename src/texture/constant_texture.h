#pragma once
#include <texture/texture.h>

NAMESPACE_BEGIN(DR)
class ConstantTexture : public Texture {
public:
  ConstantTexture(const Vector3f &albedo) : albedo_(albedo) {}
  Vector3f evalValue(const Intersection &isect) const override {
    ignore(isect);
    return albedo_;
  }

private:
  Vector3f albedo_;
};
NAMESPACE_END(DR)

#pragma once
#include <material/material.h>
#include <texture/texture.h>
NAMESPACE_BEGIN(DR)
class DielectricMaterial : public Material {
public:
  DielectricMaterial(std::shared_ptr<Texture> texture, float eta)
      : texture_(texture), eta_(eta) {
    this->specular = true;
  }
  Vector3f evalBxDF(const vec3 &r_in, const Intersection &isect,
                    const vec3 &r_out) const override;
  std::pair<Vector3f, float>
  sampleScatter(const vec3 &r_in, const Intersection &isect) const override;
  ~DielectricMaterial(){};
  Vector3f evalEmitted(const Vector3f &r_in,
                       const Intersection &isect) const override {
    ignore(r_in);
    ignore(isect);
    return {0};
  }

private:
  std::shared_ptr<Texture> texture_;
  float eta_;
};
NAMESPACE_END(DR)

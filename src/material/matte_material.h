#pragma once
#include <material/material.h>
NAMESPACE_BEGIN(DR)
class MatteMaterial : public Material
{
public:
  MatteMaterial(Vector3f albedo) : albedo_(albedo) {}
  MatteMaterial(Vector3f albedo, Vector3f emission)
      : albedo_(albedo), emission_(emission) {}
  Vector3f evalBxDF(const vec3 &r_in, const Intersection &isect,
                    const vec3 &r_out) const override;
  std::pair<Vector3f, float>
  sampleScatter(const vec3 &r_in, const Intersection &isect) const override;
  ~MatteMaterial(){};
  Vector3f evalEmitted(const Vector3f &r_in,
                       const Intersection &isect) const override {
    ignore(r_in);
    ignore(isect);
    return emission_;
  }

private:
  Vector3f albedo_;
  Vector3f emission_;
};
NAMESPACE_END(DR)

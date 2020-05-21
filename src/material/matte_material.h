#pragma once
#include <material/material.h>
NAMESPACE_BEGIN(DR)
class MatteMaterial : public Material
{
public:
  MatteMaterial(Vector3f albedo):albedo_(albedo){}
  Vector3f evalBxDF(const vec3 &r_in, const Intersection &isect,
                    const vec3 &r_out) const override;
  std::pair<Vector3f, float>
  sampleScatter(const vec3 &r_in, const Intersection &isect) const override;
  ~MatteMaterial(){};

private:
  Vector3f albedo_;
};
NAMESPACE_END(DR)

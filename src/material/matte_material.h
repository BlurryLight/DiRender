#pragma once
#include <material/material.h>
NAMESPACE_BEGIN(DR)
class MatteMaterial : public Material
{
public:
  MatteMaterial(Vector3f albedo):albedo_(albedo){}
  Vector3f evalBxDF(const Ray &r_in, const Intersection &isect, const Ray &r_out) const override;
  std::pair<Vector3f, float> evalScatter(const Ray &r_in, const Intersection &isect) const override;
  ~MatteMaterial();
private:
  Vector3f albedo_;
};
NAMESPACE_END(DR)

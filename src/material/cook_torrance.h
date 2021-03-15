//
// Created by zhong on 2021/3/15.
//

#ifndef DIRENDER_COOK_TORRANCE_H
#define DIRENDER_COOK_TORRANCE_H
#include <material/material.h>
namespace DR
{
class CookTorranceMaterial :public Material {
public:
  CookTorranceMaterial(Vector3f f0,Vector3f albedo,float roughness,float metallic,Vector3f emission={0.0f});
  Vector3f evalBxDF(const Vector3f &r_in, const Intersection &isect, const Vector3f &r_out) const override;
  Vector3f evalEmitted(const Vector3f &r_in, const Intersection &isect) const override;
  std::pair<Vector3f, float> sampleScatter(const Vector3f &r_in, const Intersection &isect) const override;
private:
  vec3 kd_;
  vec3 f0_;
  vec3 emission_;
  float roughness_;
  float metallic_;
};
}

#endif // DIRENDER_COOK_TORRANCE_H

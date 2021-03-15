#pragma once
#include <math/geometry.hpp>
#include <utils/di_global.h>
#include <cores/ray.hpp>
#include <cores/intersection.hpp>
NAMESPACE_BEGIN(DR)

struct Material
{
  virtual ~Material(){};
  virtual Vector3f evalBxDF(const Vector3f &r_in, const Intersection &isect,
                            const Vector3f &r_out) const = 0;
  virtual std::pair<Vector3f, float>
  sampleScatter(const Vector3f &r_in, const Intersection &isect) const = 0;
  virtual Vector3f evalEmitted(const Vector3f &r_in,
                               const Intersection &isect) const {
    ignore(r_in);
    ignore(isect);
    return {};
  }
  bool specular = false;
};

inline float ndf_GGX(const Vector3f& halfway,const Vector3f& normal, float roughness)
{
  //roughness :[0,1]
  roughness = clamp(0.0f,1.0f,roughness);
  float rough2 = roughness * roughness;
  float NdotH = std::max(dot(halfway,normal),0.0f);
  float NdotH2 = NdotH * NdotH;

  float nominator = rough2;
  float denominator = kPi * (NdotH2 * (rough2 - 1) + 1) * (NdotH2 * (rough2 - 1) + 1);
  return nominator / denominator;
}
inline float geometry_schlick_ggx(const Vector3f& n,const Vector3f& v, float roughness)
{
  float k = (roughness * roughness)/2;
  float NdotV = std::max(dot(n,v),0.0f);
  return NdotV / (NdotV * (1 - k) + k);
}
inline float geometry_smith(const Vector3f &n, const Vector3f &view,
                            const Vector3f &light, float roughness) {
  return geometry_schlick_ggx(n,view,roughness) * geometry_schlick_ggx(n,light,roughness);
}
inline Vector3f fresnel_schlick(const Vector3f& n,const Vector3f halfway,const Vector3f& F0,float roughness)
{
  float NdotH = std::max(dot(n,halfway),0.0f);
  return F0 + (Vector3f::max(Vector3f{1.0f - roughness},F0) - F0) * std::pow(1.0 - NdotH,5.0);
}


NAMESPACE_END(DR)

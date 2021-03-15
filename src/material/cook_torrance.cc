//
// Created by zhong on 2021/3/15.
//

#include "cook_torrance.h"
using namespace DR;
CookTorranceMaterial::CookTorranceMaterial(Vector3f f0, Vector3f albedo,
                                           float roughness, float metallic,
                                           Vector3f emission)
: kd_(albedo),f0_(lerp(f0,albedo,metallic)),emission_(emission),roughness_(roughness),metallic_(metallic)
{}
Vector3f CookTorranceMaterial::evalBxDF(const Vector3f &r_in,
                                        const Intersection &isect,
                                        const Vector3f &r_out) const {

  float NdotWo = dot(r_out,isect.normal);
  float NdotWi = dot(-r_in,isect.normal);

  if(NdotWo > 0 && NdotWi > 0)
  {
    vec3 res;
    vec3 diffuse = kd_ * k1_Pi;
    auto halfway = (-r_in + r_out).normalize();
    auto ks = fresnel_schlick(isect.normal,halfway,f0_,roughness_);
    diffuse = multiply(diffuse,(1 - ks));
    auto D = ndf_GGX(halfway,isect.normal,roughness_);
    auto F = ks;
    auto G = geometry_smith(isect.normal,-r_in,r_out,roughness_);
    res = diffuse + D * F * G / (4 * NdotWi * NdotWo);
    return res;
  }
  //invalid direction
  return {0};

}
Vector3f CookTorranceMaterial::evalEmitted(const Vector3f &r_in,
                                           const Intersection &isect) const {
  ignore(isect);
  ignore(r_in);
  return emission_;
}
std::pair<Vector3f, float>
CookTorranceMaterial::sampleScatter(const Vector3f &r_in,
                                    const Intersection &isect) const {


  ignore(r_in);
  //importance sampling
  //https://schuttejoe.github.io/post/ggximportancesamplingpart1/
  float r1 = get_random_float();
  float r2 = get_random_float();

  float theta = acos(sqrt( (1 - r1) / (r1 * (roughness_ * roughness_ - 1) + 1)));
  float sigma = 2 * kPi * r2;


  float x = sin(theta) * cos(sigma);
  float y = cos(theta);
  float z = sin(theta) * sin(sigma);
  vec3 r_out = toWorld({x, y, z}, isect.normal);

  float nom = roughness_ * roughness_ * cos(theta) * sin(theta);
  float denom = (roughness_ * roughness_ - 1) * cos(theta) * cos(theta) + 1;
  denom = kPi * denom * denom;
  float pdf = nom / denom;
  return {r_out, pdf};
}

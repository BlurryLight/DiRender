//
// Created by panda on 2021/2/19.
//

#include "phong_material.h"
using namespace DR;
std::pair<Vector3f, float>
phong_material::sampleScatter(const Vector3f &r_in,
                              const Intersection &isect) const {
  ignore(r_in);
  auto [sample_point, pdf] = cosine_sample_hemisphere();
  return {toWorld(static_cast<Vector3f>(sample_point), isect.normal), pdf};
}
phong_material::phong_material(vec3 diffuse, vec3 specular, float shininess,
                               vec3 emission, vec3 ambient)
    : diffuse_(diffuse), specular_(specular), shininess_(shininess),
      emission_(emission), ambient_(ambient) {}

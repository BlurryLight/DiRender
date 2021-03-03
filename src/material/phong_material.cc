//
// Created by panda on 2021/2/19.
//

#include "phong_material.h"
using namespace DR;
std::pair<Vector3f, float>
phong_material::sampleScatter(const Vector3f &r_in,
                              const Intersection &isect) const {

  // http://www.cs.uu.nl/docs/vakken/magr/2017-2018/slides/lecture%2013%20-%20BRDFs.pdf
  // and
  // https://www.cs.princeton.edu/courses/archive/fall03/cs526/papers/lafortune94.pdf

  float r1 = get_random_float();
  float r2 = get_random_float();
  float t = std::pow(r2, 2 / (shininess_ + 1));
  float tmp = std::sqrt(1 - t);
  float x = std::cos(kPi * 2 * r1 * tmp);
  float y = std::sin(kPi * 2 * r1 * tmp);
  float z = std::sqrt(t);

  vec3 r_out = toWorld({x, y, z}, isect.normal);
  auto idea_rout = reflect(r_in, isect.normal);
  float cos_alpha = dot(idea_rout, r_out);
  float pdf = (shininess_ + 2) * k2_Pi * std::pow(cos_alpha, shininess_);
  return {r_out, pdf};
}

std::pair<Vector3f, float>
phong_material_for_light::sampleScatter(const Vector3f &r_in,
                                        const Intersection &isect) const {
  ignore(r_in);
  auto [sample_point, pdf] = cosine_sample_hemisphere();
  return {toWorld(static_cast<Vector3f>(sample_point), isect.normal), pdf};
}
phong_material::phong_material(vec3 diffuse, vec3 specular, float shininess,
                               vec3 emission, vec3 ambient)
    : diffuse_(diffuse), specular_(specular), shininess_(shininess),
      emission_(emission), ambient_(ambient) {}
phong_material_for_light::phong_material_for_light(vec3 emission,
                                                   vec3 attenuation,
                                                   uint8_t type)
    : emission_(emission), attenuation_(attenuation), type_(type) {}

#include <material/dielectric_material.h>
using namespace DR;
Vector3f DielectricMaterial::evalBxDF(const vec3 &r_in,
                                      const Intersection &isect,
                                      const vec3 &r_out) const {
  ignore(r_in);
  ignore(r_out);
  return this->texture_->evalValue(isect);
}

std::pair<Vector3f, float>
DielectricMaterial::sampleScatter(const vec3 &r_in,
                                  const Intersection &isect) const {
  Vector3f outNormal = isect.normal;
  Vector3f refracted;
  Vector3f reflected;
  float reflected_prob;
  ignore(reflected_prob);

  float ni_over_nt = 0.0f;
  float cosine;                   // for schlick
  if (r_in.dot(isect.normal) > 0) // inner - to - outside
  {
    outNormal = -isect.normal;
    ni_over_nt = eta_;
    cosine = dot(r_in, isect.normal);
    cosine = std::sqrt(1 - eta_ * eta_ * (1 - cosine * cosine));
  } else {
    cosine = -r_in.dot(outNormal);
    ni_over_nt = 1.0 / eta_;
  }
  bool refract_flag;
  std::tie(refracted, refract_flag) = refract(r_in, outNormal, ni_over_nt);
  if (refract_flag) {
    reflected_prob = schlick(cosine, eta_);
  } else {
    reflected_prob = 1.0;
  }
  if (get_random_float(0, 0.99) < reflected_prob) {
    reflected = reflect(r_in, outNormal);
    return {reflected, 1.0f};
  }
  return {refracted, 1.0f};
}

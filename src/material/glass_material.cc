#include <material/glass_material.h>
using namespace DR;

Vector3f GlassMaterial::evalBxDF(const vec3 &r_in, const Intersection &isect,
                                 const vec3 &r_out) const {
  auto tmp = dot(reflect(r_in, isect.normal), r_out);
  if (std::fabs(tmp - 1.0f) > 1e-2) {
    return {};
  }
  return this->texture_->evalValue(isect);
}

std::pair<Vector3f, float>
GlassMaterial::sampleScatter(const vec3 &r_in,
                             const Intersection &isect) const {
  float pdf = 1.0f;
  auto r_out = reflect(r_in, isect.normal);
  return {r_out, pdf};
}

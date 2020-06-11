#include <material/glass_material.h>
using namespace DR;

Vector3f GlassMaterial::evalBxDF(const vec3 &r_in, const Intersection &isect,
                                 const vec3 &r_out) const {
  if (!almost_equal(dot(reflect(r_in, isect.normal), r_out), 1.0))
    return {};
  return this->texture_->evalValue(isect);
}

std::pair<Vector3f, float>
GlassMaterial::sampleScatter(const vec3 &r_in,
                             const Intersection &isect) const {
  float pdf = 1.0f;
  auto r_out = reflect(r_in, isect.normal);
  return {r_out, pdf};
}

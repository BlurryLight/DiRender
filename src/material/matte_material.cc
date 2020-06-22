#include <material/matte_material.h>
using namespace DR;
Vector3f MatteMaterial::evalBxDF(const vec3 &r_in, const Intersection &isect,
                                 const vec3 &r_out) const {

  ignore(r_in);
  auto tmp = dot(r_out, isect.normal);
  return tmp > 0 ? texture_->evalValue(isect) * k1_Pi : Vector3f{};
}

std::pair<Vector3f, float>
MatteMaterial::sampleScatter(const vec3 &r_in,
                             const Intersection &isect) const {
  ignore(r_in);
  auto [sample_point, pdf] = cosine_sample_hemisphere();
  return {toWorld(static_cast<Vector3f>(sample_point), isect.normal), pdf};
}

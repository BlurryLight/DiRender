#include <material/matte_material.h>
using namespace DR;
Vector3f MatteMaterial::evalBxDF(const vec3 &r_in, const Intersection &isect,
                                 const vec3 &r_out) const {

  ignore(r_in);
  auto tmp = dot(r_out, isect.normal);
  return tmp > 0 ? albedo_ * k1_Pi : Vector3f{};
}

static Vector3f toWorld(const Vector3f &a, const Vector3f &N) {
  Vector3f B, C;
  if (std::fabs(N.x) > std::fabs(N.y)) {
    float invLen = 1.0f / std::sqrt(N.x * N.x + N.z * N.z);
    C = Vector3f(N.z * invLen, 0.0f, -N.x * invLen);
  } else {
    float invLen = 1.0f / std::sqrt(N.y * N.y + N.z * N.z);
    C = Vector3f(0.0f, N.z * invLen, -N.y * invLen);
  }
  B = cross(C, N);
  return a.x * B + a.y * C + a.z * N;
}

std::pair<Vector3f, float>
MatteMaterial::sampleScatter(const vec3 &r_in,
                             const Intersection &isect) const {
  ignore(r_in);
  auto res = cosine_sample_hemisphere();
  return {toWorld(static_cast<Vector3f>(res.first), isect.normal), res.second};
}

#pragma once
#include <cores/bounds.h>
#include <functional>
#include <map>
#include <math/geometry.hpp>
#include <utils/di_global.h>

namespace DR {
class Transform;
}
namespace std {
template <> class hash<DR::Transform> {
public:
  size_t operator()(const DR::Transform &tf) const;
};
} // namespace std

NAMESPACE_BEGIN(DR)
class Transform {
public:
  Transform() : m_(Matrix4()), mInv_(Matrix4()) {}
  explicit Transform(const Matrix4 &m) : m_(m), mInv_(Matrix4::Inverse(m)) {}
  Transform(const Matrix4 &m, const Matrix4 &mInv) : m_(m), mInv_(mInv) {}
  Transform(const Transform &other, bool inverse = false)
      : m_(inverse ? other.mInv_ : other.m_),
        mInv_(inverse ? other.m_ : other.mInv_) {}
  bool operator==(const Transform &other) const;
  bool operator<(const Transform &other) const;
  static Transform Inverse(const Transform &other) {
    return Transform(other.mInv_, other.m_);
  }
  bool is_identity() const { return m_.is_identity() && mInv_.is_identity(); }
  Transform inverse(void) const { return Transform::Inverse(*this); }

  // Todo: xx
  Point3f operator()(const Point3f &other) const;

  Vector3f operator()(const Vector3f &other) const;

  Normal3f operator()(const Normal3f &other, bool reverse) const;

  Bounds3 operator()(const Bounds3 &other) const;
  Ray operator()(const Ray &other) const;

  friend size_t std::hash<Transform>::operator()(const DR::Transform &tf) const;

  // private:
  mat4 m_, mInv_;
};
NAMESPACE_END(DR)

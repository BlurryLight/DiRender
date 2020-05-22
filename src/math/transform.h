#pragma once
#include <cores/bounds.h>
#include <map>
#include <math/geometry.hpp>
#include <utils/di_global.h>

NAMESPACE_BEGIN(DR)
class Transform
{
public:
  static std::map<Transform, std::shared_ptr<Transform>> TransformTable;
  Transform() : m_(Matrix4()), mInv_(Matrix4()) {}
  Transform(const Matrix4 &m) : m_(m), mInv_(Matrix4::Inverse(m)) {}
  Transform(const Matrix4& m,const Matrix4& mInv):m_(m),mInv_(mInv){}
  bool operator==(const Transform &other) const;
  bool operator<(const Transform &other) const;
  static Transform Inverse(const Transform& other)
  {
    return Transform(other.mInv_, other.m_);
  }
  bool is_identity() const { return m_.is_identity(); }

  // Todo: xx
  Point3f operator()(const Point3f &other) const;

  Vector3f operator()(const Vector3f &other) const;

  Normal3f operator()(const Normal3f &other, bool reverse) const;

  Bounds3 operator()(const Bounds3 &other) const;
  Ray operator()(const Ray &other) const;

private:
  mat4 m_,mInv_;
};
NAMESPACE_END(DR)

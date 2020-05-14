#pragma once
#include <math/geometry.hpp>
#include <utils/di_global.h>

NAMESPACE_BEGIN(DR)
class Transform
{
public:
  Transform():m_(Matrix4()),mInv_(Matrix4()){}
  Transform(const Matrix4& m):m_(m),mInv_(Matrix4::Inverse(m)){}
  Transform(const Matrix4& m,const Matrix4& mInv):m_(m),mInv_(mInv){}
  bool operator==(const Transform& other) const
  {
    return m_ == other.m_ && mInv_ == other.mInv_;
  }
  bool operator<(const Transform &other) const { //allow the Transform to be place on std::map
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; i++) {
        if (m_.m[i][j] < other.m_.m[i][j]) {
          return true;
        }
        if (m_.m[i][j] > other.m_.m[i][j]) {
          return false;
        }
      }
    }
    return false;
  }
  static Transform Inverse(const Transform& other)
  {
    return Transform(other.mInv_,other.m_);
  }

  // Todo: xx
  Point3f operator()(const Point3f& other) const {}
private:
  mat4 m_,mInv_;
};
NAMESPACE_END(DR)

#pragma once
#include <bitset>
#include <math/geometry.hpp>
#include <utils/di_global.h>

NAMESPACE_BEGIN(DR)
class Ray {
public:
  Point3f origin_;
  Vector3f direction_;
  float tMax_;
  float tMin_;

public:
  Ray() : tMax_(kFloatInfinity), tMin_(kFloatMin) {}
  Ray(const Point3f &origin, const Vector3f &direction,
      float tMax = kFloatInfinity, float tMin = kFloatMin)
      : origin_(origin), direction_(direction) {
    if (direction_.squared_length() > 1.0f)
      direction_ = direction_.normalize();
    tMin_ = tMin;
    tMax_ = tMax;
  }
  Point3f at(float time) const { return origin_ + time * direction_; }
  bool has_nan() const {
    return origin_.has_nan() || direction_.has_nan() || std::isnan(tMax_) ||
           std::isnan(tMin_);
  }
  std::bitset<3> DirIsNeg() const {
    std::bitset<3> res;
    res[0] = (direction_[0] < 0);
    res[1] = (direction_[1] < 0);
    res[2] = (direction_[2] < 0);
    return res;
  }
};

NAMESPACE_END(DR)

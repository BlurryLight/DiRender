#include <math/transform.h>
using namespace DR;
// FIXME: the table should be managed by Scene class
// FIXME: the Entry should be
//  <Transform, const std::shared_ptr<const Transform>>
std::map<Transform, std::shared_ptr<Transform>> Transform::TransformTable =
    std::map<Transform, std::shared_ptr<Transform>>{};

bool Transform::operator==(const Transform &other) const {
  return m_ == other.m_ && mInv_ == other.mInv_;
}

bool Transform::operator<(const Transform &other)
    const { // allow the Transform to be place on std::map
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
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

Point3f Transform::operator()(const Point3f &other) const {
  float x = other.x, y = other.y, z = other.z;
  // homocoords
  float tx =
      m_.m[0][0] * x + m_.m[0][1] * y + m_.m[0][2] * z + m_.m[0][3] * 1.0f;
  float ty =
      m_.m[1][0] * x + m_.m[1][1] * y + m_.m[1][2] * z + m_.m[1][3] * 1.0f;
  float tz =
      m_.m[2][0] * x + m_.m[2][1] * y + m_.m[2][2] * z + m_.m[2][3] * 1.0f;
  return {tx, ty, tz};
}

Vector3f Transform::operator()(const Vector3f &other) const {
  float x = other.x, y = other.y, z = other.z;
  // homocoords
  float tx = m_.m[0][0] * x + m_.m[0][1] * y + m_.m[0][2] * z;
  float ty = m_.m[1][0] * x + m_.m[1][1] * y + m_.m[1][2] * z;
  float tz = m_.m[2][0] * x + m_.m[2][1] * y + m_.m[2][2] * z;
  return {tx, ty, tz};
}

Normal3f Transform::operator()(const Normal3f &other, bool reverse) const {
  float x = other.x, y = other.y, z = other.z;
  auto res =
      Normal3f(mInv_.m[0][0] * x + mInv_.m[1][0] * y + mInv_.m[2][0] * z,
               mInv_.m[0][1] * x + mInv_.m[1][1] * y + mInv_.m[2][1] * z,
               mInv_.m[0][2] * x + mInv_.m[1][2] * y + mInv_.m[2][2] * z);
  res = reverse ? -res : res;

  return res;
}

Bounds3 Transform::operator()(const Bounds3 &other) const {
  // homocoords
  // naive method
  const Transform &M = (*this);
  Bounds3 ret;
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      for (int k = 0; k < 2; k++) {
        ret =
            Bounds3::Union(ret, M(Point3f{other[i].x, other[j].y, other[k].z}));
      }
  return ret;
}

Ray Transform::operator()(const Ray &other) const {
  const Transform &M = (*this);
  Point3f o = M(other.origin_);
  Vector3f d = M(other.direction_);
  return Ray(o, d, other.tMax_, other.tMin_, false);
}

size_t std::hash<DR::Transform>::operator()(const DR::Transform &tf) {
  auto h1 = std::hash<DR::Matrix4>{}(tf.m_);
  auto h2 = std::hash<DR::Matrix4>{}(tf.mInv_);
  return h1 ^ (h2 << 1);
}

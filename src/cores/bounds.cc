#include <bitset>
#include <cores/bounds.h>
using namespace DR;

Bounds3::Bounds3() {
  pMin_ = Point3f{kFloatMax, kFloatMax, kFloatMax};
  pMax_ = Point3f{kFloatMin, kFloatMin, kFloatMin};
}

Bounds3::Bounds3(const Bounds3 &other) {
  pMin_ = other.pMin_;
  pMax_ = other.pMax_;
}

Bounds3::Bounds3(const Point3f &pMin, const Point3f &pMax) {
  pMin_ = pMin;
  pMax_ = pMax;
}

const Point3f &Bounds3::operator[](int index) const {
  if (index < 0 || index > 1)
    throw std::out_of_range("Bounds3f index out of range");
  return (index == 0) ? pMin_ : pMax_;
}

bool Bounds3::operator==(const Bounds3 &other) const {
  return (this->pMin_ == other.pMin_) && (this->pMax_ == other.pMax_);
}

bool Bounds3::operator!=(const Bounds3 &other) const {
  return !((*this) == other);
}

Vector3f Bounds3::diagonal() const { return pMax_ - pMin_; }

float Bounds3::surface_area() const {
  auto diag = this->diagonal();
  return 2 * (diag.x * diag.y + diag.x * diag.z + diag.z * diag.y);
}
float Bounds3::volume() const {
  auto diag = this->diagonal();
  return diag.x * diag.y * diag.z;
}
int Bounds3::max_extent() const {
  auto diag = this->diagonal();
  if (diag.x > diag.y && diag.x > diag.z) {
    return 0;
  } else if (diag.y > diag.z) {
    return 1;
  }
  return 2;
}

Vector3f Bounds3::centroid() const {
  auto diag = this->diagonal();
  return diag * 0.5f + static_cast<Vector3f>(pMin_);
}

Vector3f Bounds3::offset(const Point3f &p) const {
  auto v = p - pMin_;
  auto diag = this->diagonal();
  return Vector3f{v.x / diag.x, v.y / diag.y, v.z / diag.z};
}

Bounds3 Bounds3::intersect(const Bounds3 &other) const {
  return Bounds3::Intersect(*this, other);
}
bool Bounds3::is_overlapped(const Bounds3 &other) const {
  return Bounds3::Overlaps(*this, other);
}

bool Bounds3::is_inside(const Point3f &p) const {

  return Bounds3::Inside(p, *this);
}

bool Bounds3::Intersect_test(const Ray &ray, const Vector3f &invDir,
                             std::bitset<3> dirIsNeg) {
  float t_max = kFloatMax;
  float t_min = kFloatMin;
  for (int i = 0; i < 3; i++) {
    float inv_D = invDir[i];
    float t0 = (pMin_[i] - ray.origin_[i]) * inv_D;
    float t1 = (pMax_[i] - ray.origin_[i]) * inv_D;
    if (dirIsNeg[i]) {
      std::swap(t0, t1);
    }
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;
    if (t_max < t_min || t_max < 0)
      return false;
  }
  return true;
}

Bounds3 Bounds3::Union(const Bounds3 &bs, const Point3f &p) {
  return Bounds3{
      Point3f::min(bs.pMin_, p),
      Point3f::max(bs.pMax_, p),
  };
}

Bounds3 Bounds3::Union(const Bounds3 &lhs, const Bounds3 &rhs) {
  return Bounds3{
      Point3f::min(lhs.pMin_, rhs.pMin_),
      Point3f::max(lhs.pMax_, rhs.pMax_),
  };
}

Bounds3 Bounds3::Intersect(const Bounds3 &lhs, const Bounds3 &rhs) {
  return Bounds3{
      Point3f::max(lhs.pMin_, rhs.pMin_),
      Point3f::min(lhs.pMax_, rhs.pMax_),
  };
}

bool Bounds3::Overlaps(const Bounds3 &lhs, const Bounds3 &rhs) {
  bool x = (lhs.pMax_.x >= rhs.pMin_.x) && (lhs.pMin_.x <= rhs.pMax_.x);
  bool y = (lhs.pMax_.y >= rhs.pMin_.y) && (lhs.pMin_.y <= rhs.pMax_.y);
  bool z = (lhs.pMax_.z >= rhs.pMin_.z) && (lhs.pMin_.z <= rhs.pMax_.z);
  return (x && y && z);
}

bool Bounds3::Inside(const Point3f &p, const Bounds3 &bs) {
  return (p.x >= bs.pMin_.x && p.x <= bs.pMax_.x && p.y >= bs.pMin_.y &&
          p.y <= bs.pMax_.y && p.z >= bs.pMin_.z && p.z <= bs.pMax_.z);
}

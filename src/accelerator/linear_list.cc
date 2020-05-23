#include <accelerator/linear_list.h>
using namespace DR;
Bounds3 LinearList::WorldBounds() const {
  Bounds3 ret;
  for (auto i : prims_) {
    ret = Bounds3::Union(ret, i->WorldBounds());
  }
  return ret;
}

bool LinearList::Intersect_test(const Ray &ray) const {
  for (auto i : prims_) {
    if (i->Intersect_test(ray)) {
      return true;
      break;
    }
  }
  return false;
}

bool LinearList::Intersect(const Ray &ray, Intersection *isect) const {
  Intersection tmpIsect;
  Intersection tmpIsect2;
  float t_max = kFloatMax;
  bool hit = false;
  for (auto i : prims_) {
    if (i->Intersect(ray, &tmpIsect)) {
      hit = true;
      if (tmpIsect.t < t_max) {
        t_max = tmpIsect.t;
        tmpIsect2 = tmpIsect;
      }
    }
  }
  *isect = tmpIsect2;
  return hit;
}

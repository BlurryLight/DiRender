//
// Created by ravenzhong on 2021/7/20.
//

#include "normal_renderer.h"
using namespace DR;
Vector3f
NormalRenderer::cast_ray(const Ray &r, std::shared_ptr<Primitive> prim,
                         const std::vector<std::shared_ptr<Primitive>> &, int,
                         float, bool) {

  Intersection isect;
  if (!prim->Intersect(r, &isect))
    return background_;
  return isect.normal;
}
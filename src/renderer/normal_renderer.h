//
// Created by ravenzhong on 2021/7/20.
//
#pragma once
#include <renderer/path_tracing_renderer.h>
NAMESPACE_BEGIN(DR)
class NormalRenderer : public PathTracingRenderer {
public:
  NormalRenderer(uint spp) : PathTracingRenderer(1) {}
  NormalRenderer(uint spp, int nthreads)
      : PathTracingRenderer(1, nthreads) {}
  Vector3f cast_ray(const Ray &r, std::shared_ptr<Primitive> prim,
                    //we don't need below parameters
                    const std::vector<std::shared_ptr<Primitive>> &,
                    int, float, bool);
};

NAMESPACE_END(DR)

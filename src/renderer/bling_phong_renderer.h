//
// Created by panda on 2021/2/20.
//

#ifndef DIRENDER_BLING_PHONG_RENDERER_HH
#define DIRENDER_BLING_PHONG_RENDERER_HH
#pragma once
#include <renderer/path_tracing_renderer.h>
namespace DR {

class BlingPhongRenderer : public PathTracingRenderer {
public:
  BlingPhongRenderer(uint spp) : PathTracingRenderer(spp) {}
  BlingPhongRenderer(uint spp, int nthreads)
      : PathTracingRenderer(spp, nthreads) {}
  Vector3f cast_ray(const Ray &r, std::shared_ptr<Primitive> prim,
                    const std::vector<std::shared_ptr<Primitive>> &lights,
                    int depth, float, bool) override;
};

} // namespace DR
#endif // DIRENDER_BLING_PHONG_RENDERER_HH

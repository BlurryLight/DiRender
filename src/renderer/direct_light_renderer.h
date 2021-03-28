#pragma once
#include <renderer/path_tracing_renderer.h>
NAMESPACE_BEGIN(DR)
class DirectLightRenderer : public PathTracingRenderer {
public:
  DirectLightRenderer(uint spp) : PathTracingRenderer(spp) {}
  DirectLightRenderer(uint spp, int nthreads)
      : PathTracingRenderer(spp, nthreads) {}
  Vector3f cast_ray(const Ray &r, std::shared_ptr<Primitive> prim,
                    const std::vector<std::shared_ptr<Primitive>> &lights,
                    int depth, float, bool);
};

NAMESPACE_END(DR)

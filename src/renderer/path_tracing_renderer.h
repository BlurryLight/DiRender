#pragma once
#include <renderer/renderer.h>
NAMESPACE_BEGIN(DR)
class PathTracingRenderer : public Renderer {
public:
  PathTracingRenderer(uint spp) : Renderer(spp){};
  PathTracingRenderer(uint spp, int nthreads) : Renderer(spp, nthreads){};
  void render(const Scene &scene) override;

protected:
  virtual Vector3f
  cast_ray(const Ray &r, std::shared_ptr<Primitive> prim,
           const std::vector<std::shared_ptr<Primitive>> &lights, int depth);
  void render_tile(std::shared_ptr<Camera> cam, std::shared_ptr<Primitive> prim,
                   const std::vector<std::shared_ptr<Primitive>> &lights,
                   int height, int width, int blockheight, int blockwidth,
                   int blockheightId, int blockwidthId, int spp);
  Vector3f background_{0};
};
NAMESPACE_END(DR)

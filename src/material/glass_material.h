#pragma once
#include <material/material.h>
#include <texture/texture.h>
NAMESPACE_BEGIN(DR)
// perfect mirrror
class GlassMaterial : public Material {
public:
  GlassMaterial(std::shared_ptr<Texture> texture) : texture_(texture) {}
  GlassMaterial(std::shared_ptr<Texture> texture, Vector3f emission)
      : texture_(texture), emission_(emission) {}
  Vector3f evalBxDF(const vec3 &r_in, const Intersection &isect,
                    const vec3 &r_out) const override;
  std::pair<Vector3f, float>
  sampleScatter(const vec3 &r_in, const Intersection &isect) const override;
  ~GlassMaterial(){};
  Vector3f evalEmitted(const Vector3f &r_in,
                       const Intersection &isect) const override {
    ignore(r_in);
    ignore(isect);
    return emission_;
  }

private:
  std::shared_ptr<Texture> texture_;
  Vector3f emission_;
};
NAMESPACE_END(DR)

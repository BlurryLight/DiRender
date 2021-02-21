//
// Created by panda on 2021/2/19.
//
#pragma once
#include <material/material.h>
#include <texture/texture.h>
NAMESPACE_BEGIN(DR)
// Although it is similar to MatteMaterial,
// it still inherits from Material instead of MatteMaterial because it is
// relatively simple to implement.
class phong_material : public Material {
public:
  phong_material(vec3 diffuse, vec3 specular, float shininess, vec3 emission,
                 vec3 ambient);
  std::pair<Vector3f, float>
  sampleScatter(const Vector3f &r_in, const Intersection &isect) const override;

  Vector3f evalEmitted(const Vector3f &r_in,
                       const Intersection &isect) const override {
    ignore(r_in);
    ignore(isect);
    return emission_;
  }

  Vector3f diffuse_;
  Vector3f specular_;
  float shininess_;
  Vector3f emission_;
  Vector3f ambient_;

private:
  // hide
  Vector3f evalBxDF(const Vector3f &r_in, const Intersection &isect,
                    const Vector3f &r_out) const override {
    ignore(r_in);
    ignore(isect);
    ignore(r_out);
    return {};
  }
};

class phong_material_for_light : public Material {
public:
  phong_material_for_light(vec3 emission, vec3 attenuation);

  Vector3f evalEmitted(const Vector3f &r_in,
                       const Intersection &isect) const override {
    ignore(r_in);
    ignore(isect);
    return emission_;
  }
  std::pair<Vector3f, float>
  sampleScatter(const Vector3f &r_in, const Intersection &isect) const override;

  Vector3f emission_;
  Vector3f attenuation_;

private:
  // hide
  Vector3f evalBxDF(const Vector3f &r_in, const Intersection &isect,
                    const Vector3f &r_out) const override {
    ignore(r_in);
    ignore(isect);
    ignore(r_out);
    return {};
  }
};
NAMESPACE_END(DR)

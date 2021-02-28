//
// Created by panda on 2021/2/20.
//

#include "bling_phong_renderer.h"
#include <material/material.h>
#include <material/phong_material.h>
using namespace DR;
Vector3f BlingPhongRenderer::cast_ray(
    const Ray &r, std::shared_ptr<Primitive> prim,
    const std::vector<std::shared_ptr<Primitive>> &lights, int depth) {
  // All rays will be scattered to the light direction when possible
  if (depth > 5)
    return background_;
  Intersection isect;
  if (!prim->Intersect(r, &isect))
    return background_;
  auto mat_ptr = dynamic_cast<phong_material *>(isect.mat_ptr);
  if (mat_ptr) {
    Vector3f color = 0;
    for (const auto &light : lights) {
      auto light_pos = light->WorldBounds().centroid(); // center of the shape
      auto dir_vec = light_pos - isect.coords;
      Ray shadow_ray(isect.coords, dir_vec);
      Intersection shadow_isect;
      prim->Intersect(shadow_ray, &shadow_isect);
      if (shadow_isect.happened) {
        auto light_mat =
            dynamic_cast<phong_material_for_light *>(shadow_isect.mat_ptr);
        if (light_mat) {
          auto diffuse = mat_ptr->diffuse_;
          auto specular = mat_ptr->specular_;
          auto normal = isect.normal;
          assert(almost_equal(normal.length(), 1.0));
          // only support one cam
          auto view = (scene_->cams_[0]->position_ - isect.coords).normalize();
          float light_pos_distance =
              (shadow_isect.coords - isect.coords).length();
          assert(light_pos_distance > 0.0f);
          auto light_vec = (shadow_isect.coords - isect.coords).normalize();
          auto halfway_vec = (view + light_vec).normalize();

          assert(abs_almost_equal(halfway_vec.l2_norm(), 1.0));
          assert(abs_almost_equal(light_vec.l2_norm(), 1.0));
          auto tmp =
              diffuse * std::max(dot(normal, light_vec), .0f) +
              specular * std::pow(std::max(dot(normal, halfway_vec), .0f),
                                  mat_ptr->shininess_);
          auto light_mat_ptr =
              dynamic_cast<phong_material_for_light *>(shadow_isect.mat_ptr);
          auto emission = light_mat_ptr->emission_;
          assert(light_mat_ptr);
          if (light_mat_ptr->type_[0]) { // point
            auto attenuation = light_mat_ptr->attenuation_;
            auto distance_vec = vec3(1, light_pos_distance,
                                     light_pos_distance * light_pos_distance);

            color += multiply(tmp, emission / (dot(distance_vec, attenuation)));
          } else if (light_mat_ptr->type_[1]) {
            color += multiply(tmp, emission);
          }
        }
      }
    }
    return mat_ptr->ambient_ + mat_ptr->emission_ + color;
  } else {
    // the isect has been on light
    // because light material is phong_material_for_light
    // just return the emission
    return dynamic_cast<phong_material_for_light *>(isect.mat_ptr)->emission_;
  }
}

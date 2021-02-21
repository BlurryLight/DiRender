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
  try {
    auto mat_ptr = dynamic_cast<phong_material *>(isect.mat_ptr);
    Vector3f color = 0;
    for (const auto &light : lights) {
      auto [light_pos, light_pdf] = light->sample(isect.coords);
      // if return {pos,0.0}, the sample is not visible.
      if (light_pdf == 0.0f) {
        continue;
      } else {
        auto diffuse = mat_ptr->diffuse_;
        auto specular = mat_ptr->specular_;
        auto normal = isect.normal;
        assert(almost_equal(normal.length(), 1.0));
        // only support one cam
        auto view = (scene_->cams_[0]->position_ - isect.coords).normalize();
        float light_pos_distance = (light_pos.coords - isect.coords).length();
        assert(light_pos_distance > 0.0f);
        auto light_vec = (light_pos.coords - isect.coords).normalize();
        auto halfway_vec = (view + light_vec).normalize();
        auto tmp = diffuse * std::max(dot(normal, light_vec), 0.0f) +
                   specular * std::pow(std::max(dot(normal, halfway_vec), 0.0f),
                                       mat_ptr->shininess_);
        auto light_mat_ptr =
            dynamic_cast<phong_material_for_light *>(light_pos.mat_ptr);
        assert(light_mat_ptr);
        auto attenuation = light_mat_ptr->attenuation_;
        auto emission = light_mat_ptr->emission_;
        auto distance_vec = vec3(1, light_pos_distance,
                                 light_pos_distance * light_pos_distance);

        color += multiply(tmp, emission / (dot(distance_vec, attenuation)));
      }
    }
    return mat_ptr->ambient_ + mat_ptr->emission_ + color;
  } catch (const std::bad_cast &e) {
    // the isect has been on light
    // because light material is phong_material_for_light
    // just return the emission
    return dynamic_cast<phong_material_for_light *>(isect.mat_ptr)->emission_;
  }

  //  // Shadow ray
  //  Intersection light_pos;
  //  float light_pdf = 0;
  //  int size = lights.size();
  //  if (size == 0) {
  //    throw std::runtime_error("BlingPhongRenderer needs at least one
  //    light!");
  //  }
  //  // uniform sample lights
  //  int index = std::floor(size * get_random_float(0.0, 0.99));
  //  // we hope the sample is visible from the isect.coords
  //  // if return {pos,0.0}, the sample is not visible.
  //  std::tie(light_pos, light_pdf) = lights.at(index)->sample(isect.coords);
  //  Vector3f L_in = {0};
  //  Vector3f new_direction;
  //  float pdf{0};
  //  if (light_pdf == 0.0f) { // not visible
  //    ignore(pdf);
  //    return Vector3f(0.0);
  //  } else {
  //    float emit_area = 0;
  //    for (const auto &i : lights) {
  //      emit_area += i->Area();
  //    }
  //    light_pdf = 1 / emit_area;
  //    new_direction = (light_pos.coords - isect.coords).normalize();
  //    float distance2 = (light_pos.coords - isect.coords).squared_length();
  //    float cosine = std::fabs(dot(-new_direction, light_pos.normal));
  //    if (cosine < 0.01f) {
  //      return isect.mat_ptr->evalEmitted(r.direction_, isect);
  //    }
  //    pdf = distance2 * (1 / cosine) * (light_pdf);
  //  }
  //  Ray r_new(isect.coords, new_direction);
  //  auto brdf = isect.mat_ptr->evalBxDF(r.direction_, isect,
  //  r_new.direction_); L_in = isect.mat_ptr->evalEmitted(r.direction_, isect);
  //
  //  auto part1 = cast_ray(r_new, prim, lights, depth + 1);
  //  auto part2 = multiply(brdf, part1);
  //  L_in += part2 * std::max(dot(r_new.direction_, isect.normal), 0.0f) /
  //          (pdf + kEpsilon); // avoid zero
  //  return L_in;
}

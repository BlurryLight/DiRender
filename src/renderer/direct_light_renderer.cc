#include <material/material.h>
#include <renderer/direct_light_renderer.h>
#include <tuple>
using namespace DR;
Vector3f DirectLightRenderer::cast_ray(
    const Ray &r, std::shared_ptr<Primitive> prim,
    const std::vector<std::shared_ptr<Primitive>> &lights, int depth) {
  // All rays will be scattered to the light direction when possible
  if (depth > 5)
    return {0};
  Intersection isect;
  if (!prim->Intersect(r, &isect))
    return {0};

  // Shadow ray
  Intersection light_pos;
  float light_pdf = 0;
  int size = lights.size();
  // uniform sample lights
  int index = std::floor(size * get_random_float(0.0, 0.99));
  // we hope the sample is visible from the isect.coords
  // if return {pos,0.0}, the sample is not visible.
  std::tie(light_pos, light_pdf) = lights.at(index)->sample(isect.coords);
  Vector3f L_in = {0};
  Vector3f new_direction;
  float pdf;
  if (light_pdf == 0.0f) { // not visible
    ignore(pdf);
    return Vector3f(0.0);
  } else {
    float emit_area = 0;
    for (const auto &i : lights) {
      emit_area += i->Area();
    }
    light_pdf = 1 / emit_area;
    new_direction = (light_pos.coords - isect.coords).normalize();
    float distance2 = (light_pos.coords - isect.coords).squared_length();
    float cosine = std::fabs(dot(-new_direction, light_pos.normal));
    if (cosine < 0.01f) {
      return isect.mat_ptr->evalEmitted(r.direction_, isect);
    }
    pdf = distance2 * (1 / cosine) * (light_pdf);
  }
  Ray r_new(isect.coords, new_direction);
  auto brdf = isect.mat_ptr->evalBxDF(r.direction_, isect, r_new.direction_);
  L_in = isect.mat_ptr->evalEmitted(r.direction_, isect);

  auto part1 = cast_ray(r_new, prim, lights, depth + 1);
  auto part2 = multiply(brdf, part1);
  L_in += part2 * std::max(dot(r_new.direction_, isect.normal), 0.0f) /
          (pdf + kEpsilon); // avoid zero
  return L_in;
}
